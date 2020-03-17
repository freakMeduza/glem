#pragma once

#include "Buffer.hpp"

#include <memory>

namespace glem {

    class VertexArray : public Bindable {
    public:
        VertexArray();
        ~VertexArray() override;

        VertexArray(VertexArray&&) = delete;
        VertexArray(const VertexArray&) = delete;

        VertexArray& operator=(VertexArray&&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief Append vertex buffer to array
         * @param value
         */
        void append(std::unique_ptr<VertexBuffer> value) noexcept;

        /**
         * @brief Append index buffer to array
         * @param value
         */
        void append(std::unique_ptr<IndexBuffer> value) noexcept;

        /**
         * @brief Number of indices in index buffer
         * @return
         */
        size_t indexCount() const noexcept;

    private:
        uint32_t index_ {0u};

        std::vector<std::unique_ptr<VertexBuffer>> array_;

        std::unique_ptr<IndexBuffer> indexBuffer_ {nullptr};
    };

}
