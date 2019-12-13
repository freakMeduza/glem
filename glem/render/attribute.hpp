#pragma once

#include <string>

namespace glem::render {

    class Attribute {
    public:
        enum Type {
            Undefined,
            Position2D,
            Position3D,
            Texture2D,
            Color3D,
            Color4D
        };

        Attribute() = default;
        ~Attribute() = default;

        Attribute(Attribute&&) = default;
        Attribute(const Attribute&) = default;

        Attribute& operator=(Attribute&&) = default;
        Attribute& operator=(const Attribute&) = default;

        Attribute(Type type);
        Attribute(Type type, size_t offset);

        Type type() const noexcept;

        size_t size() const noexcept;
        size_t offset() const noexcept;

    private:
        friend class InputLayout;

        Type     type_   {Undefined};
        size_t   offset_ {0};

    };

}
