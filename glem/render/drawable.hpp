#pragma once

#include "visitor.hpp"
#include "bindable.hpp"

#include <vector>
#include <memory>

namespace glem::render {

    /**
     * @brief The Drawable class
     *
     * Interfasce of any drawable object
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
        bool append(const std::shared_ptr<Bindable>& value) noexcept;

    private:
        std::vector<std::shared_ptr<Bindable>> bindables_;

    };

}
