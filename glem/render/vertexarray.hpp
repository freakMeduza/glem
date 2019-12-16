#pragma once

#include "vertexbuffer.hpp"

#include <vector>
#include <memory>

namespace glem::render {

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
         * @brief append    Append render::VertexBuffer into array (enable render::VertexBuffer attributes)
         * @param value
         */
        void append(const std::shared_ptr<VertexBuffer>& value) noexcept;

    private:
        uint32_t attriuteIndex_ {0};

        std::vector<std::shared_ptr<VertexBuffer>> buffers_;

    };

}
