#include "drawable.hpp"

#include <glad/glad.h>

#include <iostream>

namespace glem::render {

    void Drawable::onAppend(IndexBuffer &value) noexcept
    {
        indexBuffer_ = &value;
    }

    void Drawable::onAppend(VertexBuffer &value) noexcept
    {
        static_cast<void>(value);
    }

    void Drawable::onAppend(VertexArray &value) noexcept
    {
        static_cast<void>(value);
    }

    void Drawable::onAppend(ShaderProgram &value) noexcept
    {
        static_cast<void>(value);
    }

    void Drawable::append(const std::shared_ptr<Bindable> &value) noexcept
    {
        value->onAppend(*this);

        bindables_.emplace_back(std::move(value));
    }

    void Drawable::draw() noexcept
    {
       for(auto&& b : bindables_)
            b->bind();

        /**** TODO: move to Renderer ****/
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexBuffer_->count()), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
    }

}
