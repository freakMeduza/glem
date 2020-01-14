#include "renderer.hpp"

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
        auto refresh = [](){
            end();
            present();
            begin();

            submitted_ = 0;
        };

        if(submitted_ == MAX_SPRITE_SIZE)
            refresh();

        const auto& position = value->position();
        const auto& color    = value->color();
        const auto& size     = value->size();

        auto uv   = value->uv();
        auto slot = -1.0f;

        if(const auto& t = value->texture()) {
            /**** choose texture unit ****/
            if(auto it = std::find_if(textures_.begin(), textures_.end(), [&t](Texture* texture){ return (t->id() == texture->id()); }); it != textures_.end())
                slot = static_cast<float>(std::distance(textures_.begin(), it));
            else {
                if(textures_.size() >= MAX_UNIT_SIZE)
                    refresh();

                textures_.push_back(t.get());

                slot = static_cast<float>(textures_.size() - 1);
            }
        }

        vertex_->position = position;
        vertex_->color    = color;
        vertex_->uv       = uv[0];
        vertex_->slot     = slot;
        vertex_++;

        vertex_->position = glm::vec3(position.x, position.y + size.y, position.z);
        vertex_->color    = color;
        vertex_->uv       = uv[1];
        vertex_->slot     = slot;
        vertex_++;

        vertex_->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
        vertex_->color    = color;
        vertex_->uv       = uv[2];
        vertex_->slot     = slot;
        vertex_++;

        vertex_->position = glm::vec3(position.x + size.x, position.y, position.z);
        vertex_->color    = color;
        vertex_->uv       = uv[3];
        vertex_->slot     = slot;
        vertex_++;

        index_ += 6;

        submitted_++;
    }

//    void Renderer::submitTest(const std::shared_ptr<Drawable> &value) noexcept
//    {
//        if(submitted_ == MAX_SPRITE_SIZE) {
//            end();
//            present();
//            begin();

//            submitted_ = 0;
//        }

//        const auto& position = value->position();
//        const auto& color    = value->color();
//        const auto& size     = value->size();

//        auto uv   = value->uv();
//        auto slot = -1.0f;

//        if(const auto& t = value->texture()) {
//            t->bind();

//            slot = t->slot();

//            uv[0] = {uv[0].x / t->numberOfRows() + value->offset().x, uv[0].y / t->numberOfRows() + value->offset().y};
//            uv[1] = {uv[1].x / t->numberOfRows() + value->offset().x, uv[1].y / t->numberOfRows() + value->offset().y};
//            uv[2] = {uv[2].x / t->numberOfRows() + value->offset().x, uv[2].y / t->numberOfRows() + value->offset().y};
//            uv[3] = {uv[3].x / t->numberOfRows() + value->offset().x, uv[3].y / t->numberOfRows() + value->offset().y};
//        }

//        vertex_->position = position;
//        vertex_->color    = color;
//        vertex_->uv       = uv[0];
//        vertex_->slot     = slot;
//        vertex_++;

//        vertex_->position = glm::vec3(position.x, position.y + size.y, position.z);
//        vertex_->color    = color;
//        vertex_->uv       = uv[1];
//        vertex_->slot     = slot;
//        vertex_++;

//        vertex_->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
//        vertex_->color    = color;
//        vertex_->uv       = uv[2];
//        vertex_->slot     = slot;
//        vertex_++;

//        vertex_->position = glm::vec3(position.x + size.x, position.y, position.z);
//        vertex_->color    = color;
//        vertex_->uv       = uv[3];
//        vertex_->slot     = slot;
//        vertex_++;

//        index_ += 6;

//        submitted_++;
//    }

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

