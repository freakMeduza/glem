#pragma once

#include <vector>
#include <memory>

#include "bindable.hpp"

namespace glem::render {

    class IndexBuffer;
    class VertexBuffer;

    /**
     * @brief The VertexArray class
     *
     * VertexArray object
     */
    class VertexArray : public Bindable {
    public:
        VertexArray();
        ~VertexArray() override;

        VertexArray(VertexArray&&) = delete;
        VertexArray(const VertexArray&) = delete;

        VertexArray& operator=(VertexArray&&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        // Bindable interface
        void bind() noexcept override;

        /**
         * @brief append        Append render::VertexBuffer into array (enable render::VertexBuffer attributes)
         * @param value
         */
        void append(const std::shared_ptr<VertexBuffer>& value) noexcept;

        /**
         * @brief append        Append render::IndexBuffer into array
         * @param value
         */
        void append(const std::shared_ptr<IndexBuffer>& value) noexcept;

        /**
         * @brief indexBuffer   render::IndexBuffer bound into array
         * @return
         */
        std::shared_ptr<IndexBuffer> indexBuffer() const noexcept;

    private:
        uint32_t attriuteIndex_ {0};

        std::shared_ptr<IndexBuffer> indexBuffer_ {nullptr};

        std::vector<std::shared_ptr<VertexBuffer>> buffers_;

    };

}
