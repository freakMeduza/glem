#include "drawable.hpp"

#include <glad/glad.h>

#include <iostream>

namespace glem::render {

    void Drawable::append(const std::shared_ptr<Bindable> &value) noexcept
    {
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
