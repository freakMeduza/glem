#pragma once

#include "Bindable.hpp"

#include <glad/glad.h>

#include <string>
#include <vector>
#include <memory>

namespace glem {

    enum class ElementType {
        Vector2f,
        Vector3f
    };

    template<ElementType Type> struct InputLayoutElementTypeMap;

    template<> struct InputLayoutElementTypeMap<ElementType::Vector2f> {
        static constexpr GLenum type  = GL_FLOAT;
        static constexpr size_t size  = sizeof(GLfloat);
        static constexpr size_t count = 2;
    };

    template<> struct InputLayoutElementTypeMap<ElementType::Vector3f> {
        static constexpr GLenum type  = GL_FLOAT;
        static constexpr size_t size  = sizeof(GLfloat);
        static constexpr size_t count = 3;
    };

    struct Element {
        std::string name;

        GLenum type   {0u};
        size_t size   {0u};
        size_t count  {0u};
        size_t offset {0u};

        bool normalized {false};
    };

    class InputLayout {
    public:
        InputLayout() = default;
        ~InputLayout() = default;

        /**
         * @brief Push element to layout
         * @param name       - Name of element
         * @param normalized - Normalized
         */
        template<ElementType Type>
        void push(const std::string& name, bool normalized = false) noexcept {
            switch(Type) {
            case ElementType::Vector2f:
                push(name,
                     InputLayoutElementTypeMap<ElementType::Vector2f>::type,
                     InputLayoutElementTypeMap<ElementType::Vector2f>::size,
                     InputLayoutElementTypeMap<ElementType::Vector2f>::count,
                     normalized);
                break;
            case ElementType::Vector3f:
                push(name,
                     InputLayoutElementTypeMap<ElementType::Vector3f>::type,
                     InputLayoutElementTypeMap<ElementType::Vector3f>::size,
                     InputLayoutElementTypeMap<ElementType::Vector3f>::count,
                     normalized);
                break;
            }
        }

        /**
         * @brief Layout
         * @return
         */
        inline const std::vector<Element>& layout() const noexcept {
            return layout_;
        }

        /**
         * @brief Layout stride
         * @return
         */
        inline size_t stride() const noexcept {
            return stride_;
        }

    private:
        void push(const std::string& name, GLenum type, size_t size, size_t count, bool normalized) noexcept {
            layout_.emplace_back(Element{name, type, size, count, stride_, normalized});
            stride_ += size * count;
        }

        size_t stride_{0u};

        std::vector<Element> layout_;

    };

    enum class BufferUsage {
        Static,
        Dynamic
    };

    template<BufferUsage Usage> struct BufferUsageMap;

    template<> struct BufferUsageMap<BufferUsage::Static> {
        static constexpr GLint usage = GL_STATIC_DRAW;
    };

    template<> struct BufferUsageMap<BufferUsage::Dynamic> {
        static constexpr GLint usage = GL_DYNAMIC_DRAW;
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
