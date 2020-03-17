#pragma once

#include <stdint.h>

namespace glem {

    class Bindable {
    public:
        virtual ~Bindable() = default;

        /**
         * @brief Bind object
         */
        virtual void bind() const noexcept = 0;

        /**
         * @brief Unbind object
         */
        virtual void unbind() const noexcept = 0;

    protected:
        uint32_t handler_ {0};

    };

}
