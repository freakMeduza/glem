#pragma once

#include "Bindable.hpp"

namespace glem {

    class Texture : public Bindable {
    public:
        Texture();
        ~Texture() override;

        Texture(Texture&&) = delete;
        Texture(const Texture&) = delete;

        Texture& operator=(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

    };

}
