#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

namespace glem {

    enum class ElementType {
        Vector2f,
        Vector3f
    };

    template<ElementType Type> struct InputLayoutElementTypeMap;

    template<> struct InputLayoutElementTypeMap<ElementType::Vector2f> {
        static constexpr size_t type  = GL_FLOAT;
        static constexpr size_t size  = sizeof(GLfloat);
        static constexpr size_t count = 2;
    };

    template<> struct InputLayoutElementTypeMap<ElementType::Vector3f> {
        static constexpr size_t type  = GL_FLOAT;
        static constexpr size_t size  = sizeof(GLfloat);
        static constexpr size_t count = 3;
    };

    struct Element {
        std::string name;

        size_t type   {0u};
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
        void push(const std::string& name, size_t type, size_t size, size_t count, bool normalized) noexcept {
            layout_.emplace_back(Element{name, type, size, count, stride_, normalized});
            stride_ += size * count;
        }

        size_t stride_{0u};

        std::vector<Element> layout_;

    };

}
