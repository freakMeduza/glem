#include "attribute.hpp"

#include <glad/glad.h>

namespace {
    [[maybe_unused]] uint32_t computeComponentCount(glem::render::AttributeType t) noexcept {
        switch (t) {
        case glem::render::AttributeType::Float:  { return 1; }
        case glem::render::AttributeType::Float2: { return 2; }
        case glem::render::AttributeType::Float3: { return 3; }
        case glem::render::AttributeType::Float4: { return 4; }
        case glem::render::AttributeType::Mat3:   { return 3 * 3; }
        case glem::render::AttributeType::Mat4:   { return 4 * 4; }
        case glem::render::AttributeType::Int:    { return 1; }
        case glem::render::AttributeType::Int2:   { return 2; }
        case glem::render::AttributeType::Int3:   { return 3; }
        case glem::render::AttributeType::Int4:   { return 4; }
        case glem::render::AttributeType::Bool:   { return 1; }
        default: { return 0; }
        }
    }

    [[maybe_unused]] uint32_t computeAttributeSize(glem::render::AttributeType t) noexcept {
        switch (t) {
        case glem::render::AttributeType::Float:  { return sizeof (uint32_t); }
        case glem::render::AttributeType::Float2: { return sizeof (uint32_t) * 2; }
        case glem::render::AttributeType::Float3: { return sizeof (uint32_t) * 3; }
        case glem::render::AttributeType::Float4: { return sizeof (uint32_t) * 4; }
        case glem::render::AttributeType::Mat3:   { return sizeof (uint32_t) * 3 * 3; }
        case glem::render::AttributeType::Mat4:   { return sizeof (uint32_t) * 4 * 4; }
        case glem::render::AttributeType::Int:    { return sizeof (uint32_t); }
        case glem::render::AttributeType::Int2:   { return sizeof (uint32_t) * 2; }
        case glem::render::AttributeType::Int3:   { return sizeof (uint32_t) * 3; }
        case glem::render::AttributeType::Int4:   { return sizeof (uint32_t) * 4; }
        case glem::render::AttributeType::Bool:   { return sizeof (uint8_t); }
        default: { return 0; }
        }
    }
}

namespace glem::render {

    Attribute::Attribute(AttributeType type, const std::string &tag, bool normalized) :
        type_       {type},
        tag_        {tag},
        offset_     {0},
        count_      {computeComponentCount(type_)},
        size_       {computeAttributeSize(type_)},
        normalized_ {normalized}
    {

    }

    AttributeType Attribute::type() const noexcept
    {
        return type_;
    }

    std::string Attribute::tag() const noexcept
    {
        return tag_;
    }

    uint32_t Attribute::offset() const noexcept
    {
        return offset_;
    }

    uint32_t Attribute::count() const noexcept
    {
        return count_;
    }

    uint32_t Attribute::size() const noexcept
    {
        return size_;
    }

    bool Attribute::normalized() const noexcept
    {
        return normalized_;
    }

}
