#pragma once

#include "attribute.hpp"

#include <vector>

namespace glem::render {

    /**
     * @brief The InputLayout class
     *
     * Describing attributes of single vertex.
     */
    class InputLayout {
    public:
        InputLayout() = default;
        ~InputLayout() = default;

        InputLayout(const std::initializer_list<Attribute>& attributes);

        InputLayout(InputLayout&&) = default;
        InputLayout(const InputLayout&) = default;

        InputLayout& operator=(InputLayout&&) = default;
        InputLayout& operator=(const InputLayout&) = default;

        /**
         * @brief stride        Verttex stride
         * @return
         */
        [[nodiscard]] uint32_t stride() const noexcept;

        /**
         * @brief attributes    Vertex attributes
         * @return
         */
        [[nodiscard]] const std::vector<Attribute>& attributes() const noexcept;

    private:
        std::vector<Attribute> attributes_;

        uint32_t stride_ {0};

    };

}
