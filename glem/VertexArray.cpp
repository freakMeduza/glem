#include "VertexArray.hpp"

#include "Log.hpp"

#include <typeinfo>

namespace {
    const std::string TAG = "VertexArray";
}

namespace glem {

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &handler_);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &handler_);
    }

    void VertexArray::bind() const noexcept
    {
        glBindVertexArray(handler_);
    }

    void VertexArray::unbind() const noexcept
    {
        glBindVertexArray(0);
    }

    void VertexArray::append(std::unique_ptr<VertexBuffer> value) noexcept
    {
        bind();
        value->bind();

        const auto& layout = value->layout();

        for(const auto& a : layout.layout()) {
            glEnableVertexAttribArray(index_);

            glVertexAttribPointer(index_,
                                  static_cast<GLint>(a.count),
                                  a.type,
                                  a.normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLsizei>(layout.stride()),
                                  reinterpret_cast<const GLvoid*>(a.offset));

            index_++;
        }

        array_.emplace_back(std::move(value));
    }

    void VertexArray::append(std::unique_ptr<IndexBuffer> value) noexcept
    {
        bind();
        value->bind();

        indexBuffer_ = std::move(value);
    }

    size_t VertexArray::indexCount() const noexcept
    {
        return indexBuffer_->size();
    }

}
