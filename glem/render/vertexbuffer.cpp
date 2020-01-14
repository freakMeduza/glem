#include "vertexbuffer.hpp"

#include <glad/glad.h>

namespace glem::render {

    VertexBuffer::VertexBuffer(InputLayout layout, const void *value, size_t size) :
        layout_ {std::move(layout)}
    {
        glCreateBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(size), value, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &id_);
    }

    void VertexBuffer::bind(uint32_t unit) noexcept
    {
        static_cast<void>(unit);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    const InputLayout &VertexBuffer::layout() const noexcept
    {
        return layout_;
    }

}
