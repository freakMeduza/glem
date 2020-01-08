#include "renderer.hpp"

#include "drawable.hpp"
#include "indexbuffer.hpp"
#include "vertexbuffer.hpp"
#include "vertexarray.hpp"

#include <util/log.hpp>

#include <glad/glad.h>

namespace {
    const std::string TAG = "Renderer";

    const uint64_t MAX_SPRITE_SIZE = 300000;

    const uint64_t VERTEX_SIZE  = sizeof (glem::render::Vertex);
    const uint64_t SPRITE_SIZE  = VERTEX_SIZE * 4;
    const uint64_t BUFFER_SIZE  = SPRITE_SIZE * MAX_SPRITE_SIZE;
    const uint64_t INDICES_SIZE = MAX_SPRITE_SIZE * 6;
}

namespace glem::render {

    std::shared_ptr<VertexArray> Renderer::vao_;
    Vertex*                      Renderer::buffer_;
    uint32_t                     Renderer::index_;

    void Renderer::init() noexcept
    {
        vao_.reset(new VertexArray{});

        auto vbo = std::make_shared<VertexBuffer>(InputLayout{
                                                      { Float3, "position" },
                                                      { Float4, "color" }
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

    void Renderer::begin() noexcept
    {
        vao_->bind();

        buffer_ = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    }

    void Renderer::submit(const std::shared_ptr<Drawable> &value) noexcept
    {
        const auto& position = value->position();
        const auto& color    = value->color();
        const auto& size     = value->size();

        buffer_->position = position;
        buffer_->color    = color;
        buffer_++;

        buffer_->position = glm::vec3(position.x, position.y + size.y, position.z);
        buffer_->color    = color;
        buffer_++;

        buffer_->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
        buffer_->color    = color;
        buffer_++;

        buffer_->position = glm::vec3(position.x + size.x, position.y, position.z);
        buffer_->color    = color;
        buffer_++;

        index_ += 6;
    }

    void Renderer::end() noexcept
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void Renderer::release() noexcept
    {
        vao_->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

        index_ = 0;
    }

}

