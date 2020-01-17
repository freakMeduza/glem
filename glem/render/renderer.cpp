#include "renderer.hpp"

#include "font.hpp"
#include "texture.hpp"
#include "drawable.hpp"
#include "indexbuffer.hpp"
#include "vertexbuffer.hpp"
#include "vertexarray.hpp"

#include <core/application.hpp>
#include <core/window.hpp>

#include <util/log.hpp>

#include <glad/glad.h>

namespace {
    const std::string TAG = "Renderer";

    const uint32_t MAX_SPRITE_SIZE = 60000;
    const uint32_t MAX_UNIT_SIZE   = 32;

    const uint32_t VERTEX_SIZE  = sizeof (glem::render::Vertex);
    const uint32_t SPRITE_SIZE  = VERTEX_SIZE * 4;
    const uint32_t BUFFER_SIZE  = SPRITE_SIZE * MAX_SPRITE_SIZE;
    const uint32_t INDICES_SIZE = MAX_SPRITE_SIZE * 6;

    [[maybe_unused]] void flush() noexcept {
        glem::render::Renderer::end();
        glem::render::Renderer::present();
        glem::render::Renderer::begin();
    }
}

namespace glem::render {

    struct RendererStorage {
        std::unique_ptr<VertexArray> vao {nullptr};

        Vertex* vertex {nullptr};

        uint32_t index     {0};
        uint32_t submitted {0};

        std::vector<std::shared_ptr<Texture>> textures;
    };

    static RendererStorage* storage {nullptr};

    void Renderer::init() noexcept
    {
        storage = new RendererStorage{};

        storage->vao.reset(new VertexArray{});

        auto vbo = std::make_shared<VertexBuffer>(InputLayout{
                                                      { Float3, "position" },
                                                      { Float4, "color"    },
                                                      { Float2, "uv"       },
                                                      { Float,  "unit"     }
                                                  },
                                                  nullptr,
                                                  BUFFER_SIZE);

        std::vector<uint32_t> indices(INDICES_SIZE, 0);

        uint32_t offset = 0;
        for(uint32_t i = 0; i < indices.size(); i += 6) {
            indices[  i  ] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        auto ibo = std::make_shared<IndexBuffer>(indices.data(), indices.size() * sizeof (uint32_t));

        storage->vao->append(std::move(vbo));
        storage->vao->append(std::move(ibo));
    }

    void Renderer::deinit() noexcept
    {
        delete storage;
        storage = nullptr;
    }

    void Renderer::begin() noexcept
    {
        storage->vao->bind();

        storage->vertex = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    }

    void Renderer::submit(const std::shared_ptr<Drawable> &value) noexcept
    {
        if(storage->submitted == MAX_SPRITE_SIZE)
            flush();

        const auto& position = value->position();
        const auto& color    = value->color();
        const auto& size     = value->size();

        auto uv   = value->uv();
        auto unit = -1.0f;

        if(const auto& t = value->texture()) {
            /**** choose texture unit ****/
            auto&& textures = storage->textures;

            if(auto it = std::find_if(textures.begin(), textures.end(), [&t](const std::shared_ptr<Texture>& texture){ return (t->id() == texture->id()); }); it != textures.end())
                unit = static_cast<float>(std::distance(textures.begin(), it));
            else {
                if(textures.size() >= MAX_UNIT_SIZE)
                    flush();

                textures.push_back(t);

                unit = static_cast<float>(textures.size() - 1);
            }
        }

        storage->vertex->position = position;
        storage->vertex->color    = color;
        storage->vertex->uv       = uv[0];
        storage->vertex->unit     = unit;
        storage->vertex++;

        storage->vertex->position = glm::vec3(position.x, position.y + size.y, position.z);
        storage->vertex->color    = color;
        storage->vertex->uv       = uv[1];
        storage->vertex->unit     = unit;
        storage->vertex++;

        storage->vertex->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
        storage->vertex->color    = color;
        storage->vertex->uv       = uv[2];
        storage->vertex->unit     = unit;
        storage->vertex++;

        storage->vertex->position = glm::vec3(position.x + size.x, position.y, position.z);
        storage->vertex->color    = color;
        storage->vertex->uv       = uv[3];
        storage->vertex->unit     = unit;
        storage->vertex++;

        storage->index += 6;

        storage->submitted++;
    }

    void Renderer::submitText(const std::string &text, const glm::vec2 &position, const Font &font, const glm::vec4 &color, float scale) noexcept
    {
        if(storage->submitted == MAX_SPRITE_SIZE)
            flush();

        auto unit = -1.0f;

        if(const auto& t = font.atlas()) {
            /**** choose texture unit ****/
            auto&& textures = storage->textures;

            if(auto it = std::find_if(textures.begin(), textures.end(), [&t](const std::shared_ptr<Texture>& texture){ return (t->id() == texture->id()); }); it != textures.end())
                unit = static_cast<float>(std::distance(textures.begin(), it));
            else {
                if(textures.size() >= MAX_UNIT_SIZE)
                    flush();

                textures.push_back(t);

                unit = static_cast<float>(textures.size() - 1);
            }
        }

        auto pen_x = position.x;
        auto pen_y = position.y;

        for(uint32_t i = 0; i < text.size(); ++i) {
            const auto& ascii = text[i];

            if(auto it = std::find_if(font.glyphs().begin(),
                                      font.glyphs().end(),
                                      [character = ascii](const Glyph& glyph){ return (character == glyph.ascii); });
                    it != font.glyphs().end()) {
                float x = pen_x + (*it).bearing.x * scale;
                float y = -pen_y - (*it).bearing.y * scale;

                float width  = (*it).size.x * scale;
                float height = (*it).size.y * scale;

                storage->vertex->position = glm::vec3{x, -y, 0.0f};
                storage->vertex->color    = color;
                storage->vertex->uv       = glm::vec2{(*it).uv.x, (*it).uv.y};
                storage->vertex->unit     = unit;
                storage->vertex++;

                storage->vertex->position = glm::vec3{x, -y - height, 0.0f};
                storage->vertex->color    = color;
                storage->vertex->uv       = glm::vec2{(*it).uv.x, (*it).uv.y + (*it).size.y / font.atlas()->height()};
                storage->vertex->unit     = unit;
                storage->vertex++;

                storage->vertex->position = glm::vec3{x + width, -y - height, 0.0f};
                storage->vertex->color    = color;
                storage->vertex->uv       = glm::vec2{(*it).uv.x + (*it).size.x / font.atlas()->width(), (*it).uv.y + (*it).size.y / font.atlas()->height()};
                storage->vertex->unit     = unit;
                storage->vertex++;

                storage->vertex->position = glm::vec3{x + width, -y, 0.0f};
                storage->vertex->color    = color;
                storage->vertex->uv       = glm::vec2{(*it).uv.x + (*it).size.x / font.atlas()->width(), (*it).uv.y};
                storage->vertex->unit     = unit;
                storage->vertex++;

                pen_x += (*it).advance.x * scale;
                pen_y += (*it).advance.y * scale;

                storage->index += 6;

                storage->submitted++;
            }
        }

    }

    void Renderer::end() noexcept
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void Renderer::present() noexcept
    {
        for(uint32_t i = 0; i < storage->textures.size(); ++i)
            storage->textures[i]->bind(i);

        storage->vao->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(storage->index), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

        storage->index = 0;
        storage->submitted = 0;
    }

}

