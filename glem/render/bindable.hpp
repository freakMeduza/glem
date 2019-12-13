#pragma once

#include <stdint.h>

namespace glem::render {

    class Bindable {
    public:
        Bindable() = default;
        virtual ~Bindable() = default;

        virtual void bind()   noexcept = 0;

    protected:
        uint32_t id_ {0};

    };

}
