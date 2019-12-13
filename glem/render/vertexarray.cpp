#include "vertexarray.hpp"

#include <glad/glad.h>

namespace glem::render {

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &id_);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &id_);
    }

    void VertexArray::bind() noexcept
    {
        glBindVertexArray(id_);
    }

    void VertexArray::append(const std::shared_ptr<Bindable> &value) noexcept
    {
        bind();
        value->bind();

        /**** TODO: magic with buffer layout ****/

        bindables_.emplace_back(value);
    }

}
