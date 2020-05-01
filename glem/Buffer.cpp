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

    const VertexLayout &VertexBuffer::vLayout() const noexcept
    {
        return vLayout_;
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

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &handler_);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &handler_);
    }

    void VertexArray::bind() const noexcept
    {
        glBindVertexArray(handler_);
    }

    void VertexArray::unbind() const noexcept
    {
        glBindVertexArray(0);
    }

    void VertexArray::append(std::unique_ptr<VertexBuffer> value) noexcept
    {
        bind();
        value->bind();

        const auto& layout = value->vLayout();

        for(const auto& attr : layout.attributes()) {
            glEnableVertexAttribArray(index_);

            GLenum type {GL_NONE};

            switch (attr.type()) {
            case AttributeType::Vector2f:
                type = AttributeTypeMap<AttributeType::Vector2f>::systemType;
                break;
            case AttributeType::Vector3f:
                type = AttributeTypeMap<AttributeType::Vector3f>::systemType;
                break;
            }

            glVertexAttribPointer(index_,
                                  static_cast<GLint>(attr.count()),
                                  type,
                                  GL_FALSE,
                                  static_cast<GLsizei>(layout.size()),
                                  reinterpret_cast<const GLvoid*>(attr.offset()));

            index_++;
        }

//        const auto& layout = value->layout();

//        for(const auto& a : layout.layout()) {
//            glEnableVertexAttribArray(index_);

//            glVertexAttribPointer(index_,
//                                  static_cast<GLint>(a.count),
//                                  a.type,
//                                  a.normalized ? GL_TRUE : GL_FALSE,
//                                  static_cast<GLsizei>(layout.stride()),
//                                  reinterpret_cast<const GLvoid*>(a.offset));

//            index_++;
//        }

        array_.emplace_back(std::move(value));
    }

    void VertexArray::append(std::unique_ptr<IndexBuffer> value) noexcept
    {
        bind();
        value->bind();

        indexBuffer_ = std::move(value);
    }

    size_t VertexArray::indexCount() const noexcept
    {
        return indexBuffer_->size();
    }

}
