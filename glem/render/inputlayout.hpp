#pragma once

#include "attribute.hpp"

#include <vector>

namespace glem::render {

    class InputLayout {
    public:
        InputLayout() = default;
        ~InputLayout() = default;

        InputLayout(const std::initializer_list<Attribute>& attributes);

        /**
         * @brief stride Vertex stride
         * @return
         */
        size_t stride() const noexcept;

        /**
         * @brief attributes Vertex attributes
         * @return
         */
        const std::vector<Attribute>& attributes() const noexcept;

    private:
        std::vector<Attribute> attributes_;

        size_t stride_;

    };

}
