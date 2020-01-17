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

    const uint32_t MAX_SPRITE_SIZE = 50000;
    const uint32_t MAX_UNIT_SIZE   = 32;

    const uint32_t VERTEX_SIZE  = sizeof (glem::render::Vertex);
    const uint32_t SPRITE_SIZE  = VERTEX_SIZE * 4;
    const uint32_t BUFFER_SIZE  = SPRITE_SIZE * MAX_SPRITE_SIZE;
    const uint32_t INDICES_SIZE = MAX_SPRITE_SIZE * 6;
}

namespace glem::render {

    VertexArray*          Renderer::vao_       {nullptr};
    Vertex*               Renderer::vertex_    {nullptr};
    uint32_t              Renderer::index_     {0};
    uint32_t              Renderer::submitted_ {0};
    std::vector<Texture*> Renderer::textures_;

    void Renderer::init() noexcept
    {
        vao_ = new VertexArray{};

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

        vao_->append(vbo);
        vao_->append(ibo);
    }

    void Renderer::deinit() noexcept
    {
        delete vao_;
        vao_ = nullptr;
    }

    void Renderer::begin() noexcept
    {
        vao_->bind();

        vertex_ = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    }

    void Renderer::submit(const std::shared_ptr<Drawable> &value) noexcept
    {
        auto flush = [](){
            end();
            present();
            begin();

            submitted_ = 0;
        };

        if(submitted_ == MAX_SPRITE_SIZE)
            flush();

        const auto& position = value->position();
        const auto& color    = value->color();
        const auto& size     = value->size();

        auto uv   = value->uv();
        auto unit = -1.0f;

        if(const auto& t = value->texture()) {
            /**** choose texture unit ****/
            if(auto it = std::find_if(textures_.begin(), textures_.end(), [&t](Texture* texture){ return (t->id() == texture->id()); }); it != textures_.end())
                unit = static_cast<float>(std::distance(textures_.begin(), it));
            else {
                if(textures_.size() >= MAX_UNIT_SIZE)
                    flush();

                textures_.push_back(t.get());

                unit = static_cast<float>(textures_.size() - 1);
            }
        }

        vertex_->position = position;
        vertex_->color    = color;
        vertex_->uv       = uv[0];
        vertex_->unit     = unit;
        vertex_++;

        vertex_->position = glm::vec3(position.x, position.y + size.y, position.z);
        vertex_->color    = color;
        vertex_->uv       = uv[1];
        vertex_->unit     = unit;
        vertex_++;

        vertex_->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
        vertex_->color    = color;
        vertex_->uv       = uv[2];
        vertex_->unit     = unit;
        vertex_++;

        vertex_->position = glm::vec3(position.x + size.x, position.y, position.z);
        vertex_->color    = color;
        vertex_->uv       = uv[3];
        vertex_->unit     = unit;
        vertex_++;

        index_ += 6;

        submitted_++;
    }

    void Renderer::submitText(const std::string &text, const glm::vec2 &position, const Font &font, const glm::vec4 &color, float scale) noexcept
    {
        auto flush = [](){
            end();
            present();
            begin();

            submitted_ = 0;
        };

        if(submitted_ == MAX_SPRITE_SIZE)
            flush();

        auto unit = -1.0f;

        if(const auto& t = font.atlas()) {
            /**** choose texture unit ****/
            if(auto it = std::find_if(textures_.begin(), textures_.end(), [&t](Texture* texture){ return (t->id() == texture->id()); }); it != textures_.end())
                unit = static_cast<float>(std::distance(textures_.begin(), it));
            else {
                if(textures_.size() >= MAX_UNIT_SIZE)
                    flush();

                textures_.push_back(t.get());

                unit = static_cast<float>(textures_.size() - 1);
            }
        }
        else
            util::Log::w(TAG, "Failed to find texture in atlas.");


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

                vertex_->position = glm::vec3{x, -y, 0.0f};
                vertex_->color    = color;
                vertex_->uv       = glm::vec2{(*it).uv.x, (*it).uv.y};
                vertex_->unit     = unit;
                vertex_++;

                vertex_->position = glm::vec3{x, -y - height, 0.0f};
                vertex_->color    = color;
                vertex_->uv       = glm::vec2{(*it).uv.x, (*it).uv.y + (*it).size.y / font.atlas()->height()};
                vertex_->unit     = unit;
                vertex_++;

                vertex_->position = glm::vec3{x + width, -y - height, 0.0f};
                vertex_->color    = color;
                vertex_->uv       = glm::vec2{(*it).uv.x + (*it).size.x / font.atlas()->width(), (*it).uv.y + (*it).size.y / font.atlas()->height()};
                vertex_->unit     = unit;
                vertex_++;

                vertex_->position = glm::vec3{x + width, -y, 0.0f};
                vertex_->color    = color;
                vertex_->uv       = glm::vec2{(*it).uv.x + (*it).size.x / font.atlas()->width(), (*it).uv.y};
                vertex_->unit     = unit;
                vertex_++;

                pen_x += (*it).advance.x * scale;
                pen_y += (*it).advance.y * scale;

                index_ += 6;

                submitted_++;
            }
        }
    }

    void Renderer::end() noexcept
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void Renderer::present() noexcept
    {
        for(uint32_t i = 0; i < textures_.size(); ++i)
            textures_[i]->bind(i);

        vao_->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

        index_ = 0;
    }

}

