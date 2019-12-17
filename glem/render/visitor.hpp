#pragma once

#include <memory>

namespace glem::render {

    struct Visitor {
        Visitor() = default;
        virtual ~Visitor() = default;

        virtual void onAppend(class IndexBuffer&   value) noexcept = 0;
        virtual void onAppend(class VertexBuffer&  value) noexcept = 0;
        virtual void onAppend(class VertexArray&   value) noexcept = 0;
        virtual void onAppend(class ShaderProgram& value) noexcept = 0;

    };

}
