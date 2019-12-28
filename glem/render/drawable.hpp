#pragma once

#include "context.hpp"
#include "bindable.hpp"

#include "vertexarray.hpp"

#include <vector>
#include <memory>

namespace glem::render {

    /**
     * @brief The Drawable class
     *
     * Interface of any drawable object
     */
    class Drawable {
    public:
        Drawable() = default;
        virtual ~Drawable() = default;

        Drawable(Drawable&&) = delete;
        Drawable(const Drawable&) = delete;

        Drawable& operator=(Drawable&&) = delete;
        Drawable& operator=(const Drawable&) = delete;

        /**
         * @brief append  Append bindable
         * @param value
         */
        void append(const std::shared_ptr<Bindable>& value) noexcept;

        /**
         * @brief append
         * @param value
         */
        void append(const std::shared_ptr<VertexArray>& value) noexcept;

        /**
         * @brief draw
         */
        void draw() noexcept;

    private:
        std::vector<std::shared_ptr<Bindable>> bindables_;

        IndexBuffer* indexBuffer_ {nullptr};


    };

}
