#include "drawable.hpp"

#include <glad/glad.h>

namespace glem::render {

    void Drawable::append(const std::shared_ptr<Bindable> &value) noexcept
    {
        bindables_.emplace_back(std::move(value));
    }

    void Drawable::append(const std::shared_ptr<VertexArray> &value) noexcept
    {
        indexBuffer_ = value->indexBuffer().get();

        bindables_.emplace_back(std::move(value));
    }

    void Drawable::draw() noexcept
    {
       for(auto&& b : bindables_)
            b->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexBuffer_->count()), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
    }

}
