#include "vertexbuffer.hpp"

#include <glad/glad.h>

namespace glem::render {

    VertexBuffer::VertexBuffer(const InputLayout &layout) :
        layout_ {std::move(layout)}
    {
        glCreateBuffers(1, &id_);
    }

    VertexBuffer::VertexBuffer(const InputLayout &layout, const void *value, uint32_t size) :
        layout_ {std::move(layout)}
    {
        glCreateBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, size, value, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &id_);
    }

    void VertexBuffer::bind() noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    const InputLayout &VertexBuffer::layout() const noexcept
    {
        return layout_;
    }

}
