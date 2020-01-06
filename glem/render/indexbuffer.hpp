#pragma once

#include "bindable.hpp"

namespace glem::render {

    /**
     * @brief The IndexBuffer class
     *
     * Describing buffer of indices
     */
    class IndexBuffer : public Bindable {
    public:
        IndexBuffer();
        IndexBuffer(const void* value,
                    size_t size);

        ~IndexBuffer() override;

        IndexBuffer(IndexBuffer&&) = delete;
        IndexBuffer(const IndexBuffer&) = delete;

        IndexBuffer& operator=(IndexBuffer&&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        // Bindable interface
        void bind() noexcept override;

        /**
         * @brief count    Count of indicies
         * @return
         */
        [[nodiscard]] uint32_t count() const noexcept;

    private:
        uint32_t count_ {0};

    };

}
