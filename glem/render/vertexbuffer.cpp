#include "vertexbuffer.hpp"

#include <glad/glad.h>

namespace glem::render {

    VertexBuffer::VertexBuffer()
    {
        glCreateBuffers(1, &id_);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &id_);
    }

    void VertexBuffer::bind() noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

}
