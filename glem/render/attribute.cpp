#include "attribute.hpp"

#include <glad/glad.h>

namespace glem::render {

    Attribute::Attribute(Attribute::Type type) :
        type_ {type}
    {

    }

    Attribute::Attribute(Attribute::Type type, size_t offset) :
        type_   {type},
        offset_ {offset}
    {

    }

    Attribute::Type Attribute::type() const noexcept
    {
        return type_;
    }

    size_t Attribute::size() const noexcept
    {
        switch (type_) {
        case Type::Position2D: { return sizeof (GLfloat) * 2; }
        case Type::Position3D: { return sizeof (GLfloat) * 3; }
        case Type::Texture2D:  { return sizeof (GLfloat) * 2; }
        case Type::Color3D:    { return sizeof (GLfloat) * 3; }
        case Type::Color4D:    { return sizeof (GLfloat) * 4; }
        default: return 0;
        }
    }

    size_t Attribute::offset() const noexcept
    {
        return offset_;
    }

}
