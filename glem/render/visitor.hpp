#pragma once

#include <memory>

namespace glem::render {

    struct Visitor {
        Visitor() = default;
        virtual ~Visitor() = default;

        virtual void visit(class IndexBuffer&   value) noexcept = 0;
        virtual void visit(class VertexBuffer&  value) noexcept = 0;
        virtual void visit(class VertexArray&   value) noexcept = 0;
        virtual void visit(class ShaderProgram& value) noexcept = 0;

    };

}
