#include "indexbuffer.hpp"

#include <glad/glad.h>

namespace glem::render {

    IndexBuffer::IndexBuffer()
    {
        glCreateBuffers(1, &id_);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &id_);
    }

    void IndexBuffer::bind() noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

}
