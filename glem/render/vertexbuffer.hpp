#pragma once

#include "bindable.hpp"
#include "inputlayout.hpp"

namespace glem::render {

    /**
     * @brief The VertexBuffer class
     *
     * Descibing buffer of vertices
     */
    class VertexBuffer : public Bindable {
    public:
        VertexBuffer(InputLayout layout,
                     const void* value,
                     size_t size);

        ~VertexBuffer() override;

        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer(const VertexBuffer&) = delete;

        VertexBuffer& operator=(VertexBuffer&&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        // Bindable interface
        void bind(uint32_t unit = 0) noexcept override;

        /**
         * @brief layout    render::InputLayout
         * @return
         */
        [[nodiscard]] const InputLayout& layout() const noexcept;

    private:
        InputLayout layout_;

    };

}
