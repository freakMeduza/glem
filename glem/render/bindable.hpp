#pragma once

#include <stdint.h>

#include "visitor.hpp"

namespace glem::render {

    /**
     * @brief The Bindable class
     *
     * Interface of any bindable object
     */
    class Bindable {
    public:
        Bindable() = default;
        virtual ~Bindable() = default;

        /**
         * @brief bind      Bind object
         */
        virtual void bind() noexcept = 0;

        /**
         * @brief onAppend  Slot on append
         * @param visitor
         */
        virtual void onAppend(Visitor& visitor) noexcept = 0;

    protected:
        uint32_t id_ {0};

    };

}
