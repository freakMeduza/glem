#pragma once

#include "bindable.hpp"

#include <vector>
#include <memory>

namespace glem::render {

    class VertexArray : public Bindable {
    public:
        VertexArray();
        ~VertexArray() override;

        // Bindable interface
        void bind() noexcept override;

        void append(const std::shared_ptr<Bindable>& value) noexcept;

    private:
        std::vector<std::shared_ptr<Bindable>> bindables_;

    };

}
