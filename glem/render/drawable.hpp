#pragma once

#include "context.hpp"
#include "indexbuffer.hpp"
#include "vertexarray.hpp"

#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

namespace glem::render {

    class Drawable {
    public:
        Drawable() = default;
        virtual ~Drawable() = default;

        Drawable(const Drawable&) = delete;
        Drawable& operator=(const Drawable&) = delete;

        /**
         * @brief modelMatrix Transform (rotation, scale, etc.)
         * @return
         */
        virtual glm::mat4 modelMatrix() const noexcept = 0;

        void draw(Context& context) const noexcept;

    private:
        std::shared_ptr<IndexBuffer> indexBuffer_ {nullptr};

        std::vector<std::shared_ptr<Bindable>> bindables_;

    };

}
