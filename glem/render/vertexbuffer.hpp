#pragma once

#include "bindable.hpp"

namespace glem::render {

    class VertexBuffer : public Bindable {
    public:
        VertexBuffer();
        ~VertexBuffer() override;

        // Bindable interface
        void bind() noexcept override;

    };

}
