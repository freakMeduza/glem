#include "indexbuffer.hpp"

#include <glad/glad.h>

namespace glem::render {

    IndexBuffer::IndexBuffer()
    {
        glCreateBuffers(1, &id_);
    }

    IndexBuffer::IndexBuffer(const void* value, size_t size)
    {
        glCreateBuffers(1, &id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(size), value, GL_STATIC_DRAW);

        count_ = static_cast<uint32_t>(size / sizeof (uint32_t));
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &id_);
    }

    void IndexBuffer::bind() noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    uint32_t IndexBuffer::count() const noexcept
    {
        return count_;
    }

}
