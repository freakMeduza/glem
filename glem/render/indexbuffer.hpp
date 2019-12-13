#pragma once

#include "bindable.hpp"

namespace glem::render {

    class IndexBuffer : public Bindable {
    public:
        IndexBuffer();
        ~IndexBuffer() override;

        // Bindable interface
        void bind() noexcept override;

    };

}
