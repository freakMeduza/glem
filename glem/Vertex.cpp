#include "Vertex.hpp"

#include "Log.hpp"

#include <algorithm>

namespace {
    static constexpr const char* TAG = "Vertex";
}

namespace glem {

    Attribute::Attribute(AttributeType type, const std::string& semantic, size_t offset) :
        type_{type}, semantic_{semantic}, offset_{offset}
    {

    }

    AttributeType Attribute::type() const noexcept
    {
        return type_;
    }

    std::string Attribute::semantic() const noexcept
    {
        return semantic_;
    }

    size_t Attribute::size() const noexcept
    {
        switch (type_) {
        case AttributeType::Vector2f:
            return AttributeTypeMap<AttributeType::Vector2f>::size;
        case AttributeType::Vector3f:
            return AttributeTypeMap<AttributeType::Vector3f>::size;
        }

        Log::e(TAG, "Unsupported attribute type.");

        return 0u;
    }

    size_t Attribute::count() const noexcept
    {
        switch (type_) {
        case AttributeType::Vector2f:
            return AttributeTypeMap<AttributeType::Vector2f>::count;
        case AttributeType::Vector3f:
            return AttributeTypeMap<AttributeType::Vector3f>::count;
        }

        Log::e(TAG, "Unsupported attribute type.");

        return 0u;
    }

    size_t Attribute::offset() const noexcept
    {
        return offset_;
    }

    VertexLayout &VertexLayout::push(AttributeType type, const std::string& semantic) noexcept
    {
        layout_.emplace_back(type, semantic, size_);

        size_ += layout_.back().size() * layout_.back().count();

        return (*this);
    }

    const Attribute &VertexLayout::attribute(const std::string &value) const
    {
        for(const auto& attr : layout_)
            if(attr.semantic() == value)
                return attr;

        throw std::runtime_error("Couldn't find attribute.");

        return layout_.front();
    }

    const Attribute &VertexLayout::attributeByIndex(size_t value) const
    {
        return layout_[value];
    }

    size_t VertexLayout::size() const noexcept
    {
        return size_;
    }

    size_t VertexLayout::count() const noexcept
    {
        return layout_.size();
    }

    const std::vector<Attribute> &VertexLayout::attributes() const noexcept
    {
        return layout_;
    }

    VertexByteBuffer::VertexByteBuffer(const VertexLayout &layout) :
        layout_{layout}
    {

    }

    const uint8_t *VertexByteBuffer::data() const noexcept
    {
        return buffer_.data();
    }

    const VertexLayout &VertexByteBuffer::layout() const noexcept
    {
        return layout_;
    }

    size_t VertexByteBuffer::size() const noexcept
    {
        return buffer_.size();
    }

    size_t VertexByteBuffer::count() const noexcept
    {
        return buffer_.size() / layout_.size();
    }

    Vertex VertexByteBuffer::front() const
    {
        if(buffer_.size() == 0)
            throw std::runtime_error("Buffer is empty.");

        return Vertex{const_cast<uint8_t*>(buffer_.data()), layout_};
    }

    Vertex VertexByteBuffer::back() const
    {
        if(buffer_.size() == 0)
            throw std::runtime_error("Buffer is empty.");

        return Vertex{const_cast<uint8_t*>(buffer_.data() + buffer_.size() - layout_.size()), layout_};
    }

    Vertex::Vertex(uint8_t *data, const VertexLayout &layout) :
        layout_{layout}, data_{data}
    {
        if(!data_)
            throw std::runtime_error("Vertex data is empty.");
    }

}
