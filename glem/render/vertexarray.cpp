#include "vertexarray.hpp"

#include <glad/glad.h>

#include <iostream>

namespace {
    [[maybe_unused]] GLenum translate(glem::render::AttributeType t) noexcept {
        switch (t) {
        case glem::render::AttributeType::Float:  { return GL_FLOAT; }
        case glem::render::AttributeType::Float2: { return GL_FLOAT; }
        case glem::render::AttributeType::Float3: { return GL_FLOAT; }
        case glem::render::AttributeType::Float4: { return GL_FLOAT; }
        case glem::render::AttributeType::Mat3:   { return GL_FLOAT; }
        case glem::render::AttributeType::Mat4:   { return GL_FLOAT; }
        case glem::render::AttributeType::Int:    { return GL_INT;   }
        case glem::render::AttributeType::Int2:   { return GL_INT;   }
        case glem::render::AttributeType::Int3:   { return GL_INT;   }
        case glem::render::AttributeType::Int4:   { return GL_INT;   }
        case glem::render::AttributeType::Bool:   { return GL_BOOL;  }
        default: return GL_FALSE;
        }
    }
}

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

    void VertexArray::onAppend(Visitor &visitor) noexcept
    {
        visitor.onAppend(*this);
    }

    void VertexArray::append(const std::shared_ptr<VertexBuffer> &value) noexcept
    {
        bind();
        value->bind();

        const auto& layout = value->layout();

        for(const auto& attr : layout.attributes()) {
            std::cout << attr.tag() << std::endl;
            std::cout << "offset: "   << attr.offset() << std::endl;
            std::cout << "count:  "   << attr.count()  << std::endl;
            std::cout << "size:   "   << attr.size()   << std::endl;

            glEnableVertexAttribArray(attriuteIndex_);

            glVertexAttribPointer(attriuteIndex_,
                                  static_cast<GLint>(attr.count()),
                                  translate(attr.type()),
                                  attr.normalized() ? GL_TRUE : GL_FALSE,
                                  static_cast<GLsizei>(layout.stride()),
                                  reinterpret_cast<const void*>(attr.offset()));

            attriuteIndex_++;
        }

        buffers_.emplace_back(std::move(value));
    }

}
