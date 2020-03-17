#include "Buffer.hpp"

namespace glem {

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &handler_);
    }

    void VertexBuffer::bind() const noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, handler_);
    }

    void VertexBuffer::unbind() const noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const InputLayout &VertexBuffer::layout() const noexcept
    {
        return layout_;
    }

    IndexBuffer::IndexBuffer(const std::vector<uint32_t> &data, BufferUsage usage) :
        size_{data.size()}, usage_{usage}
    {
        glCreateBuffers(1, &handler_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handler_);

        switch (usage) {
        case BufferUsage::Static:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof (uint32_t), data.data(), BufferUsageMap<BufferUsage::Static>::usage);
            break;
        case BufferUsage::Dynamic:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof (uint32_t), data.data(), BufferUsageMap<BufferUsage::Dynamic>::usage);
            break;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &handler_);
    }

    void IndexBuffer::bind() const noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handler_);
    }

    void IndexBuffer::unbind() const noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t IndexBuffer::size() const noexcept
    {
        return size_;
    }

}
