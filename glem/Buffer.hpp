#pragma once

#include "Bindable.hpp"
#include "InputLayout.hpp"

#include <glad/glad.h>

namespace glem {

    enum class BufferUsage {
        Static,
        Dynamic
    };

    template<BufferUsage Usage> struct BufferUsageMap;

    template<> struct BufferUsageMap<BufferUsage::Static> {
        static constexpr size_t usage = GL_STATIC_DRAW;
    };

    template<> struct BufferUsageMap<BufferUsage::Dynamic> {
        static constexpr size_t usage = GL_DYNAMIC_DRAW;
    };

    class VertexBuffer : public Bindable {
    public:
        template<typename T>
        VertexBuffer(const std::vector<T>& data, const InputLayout& layout, BufferUsage usage = BufferUsage::Static) :
            layout_{layout}, usage_{usage}
        {
            glCreateBuffers(1, &handler_);

            glBindBuffer(GL_ARRAY_BUFFER, handler_);

            switch (usage) {
            case BufferUsage::Static:
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof (T), data.data(), BufferUsageMap<BufferUsage::Static>::usage);
                break;
            case BufferUsage::Dynamic:
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof (T), data.data(), BufferUsageMap<BufferUsage::Dynamic>::usage);
                break;
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        ~VertexBuffer() override;

        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer(const VertexBuffer&) = delete;

        VertexBuffer& operator=(VertexBuffer&&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief Input layout
         * @return
         */
        const InputLayout& layout() const noexcept;

    private:
        InputLayout layout_;
        BufferUsage usage_;

    };

    class IndexBuffer : public Bindable {
    public:
        IndexBuffer(const std::vector<uint32_t>& data, BufferUsage usage = BufferUsage::Static);
        ~IndexBuffer() override;

        IndexBuffer(IndexBuffer&&) = delete;
        IndexBuffer(const IndexBuffer&) = delete;

        IndexBuffer& operator=(IndexBuffer&&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief size
         * @return
         */
        size_t size() const noexcept;

    private:
        size_t size_ {0u};

        BufferUsage usage_;

    };

}
