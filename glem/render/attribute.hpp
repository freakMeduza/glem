#pragma once

#include <string>

namespace glem::render {

    enum AttributeType {
        Undefined,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    /**
     * @brief The Attribute class
     *
     * Describe single vertex attribute
     */
    class Attribute {
    public:
        Attribute() = default;
        ~Attribute() = default;

        Attribute(AttributeType type,
                  const std::string& tag,
                  bool normalized = false);

        Attribute(Attribute&&) = default;
        Attribute(const Attribute&) = default;

        Attribute& operator=(Attribute&&) = default;
        Attribute& operator=(const Attribute&) = default;

        /**
         * @brief type      Attribute type (enum AttributeType)
         * @return
         */
        [[nodiscard]] AttributeType type() const noexcept;

        /**
         * @brief tag       Attribute debug tag
         * @return
         */
        [[nodiscard]] std::string   tag()  const noexcept;

        /**
         * @brief offset    Attribute offset
         * @return
         */
        [[nodiscard]] uint32_t offset() const noexcept;

        /**
         * @brief count     Attribute component count
         * @return
         */
        [[nodiscard]] uint32_t count()  const noexcept;

        /**
         * @brief size      Attribute type size
         * @return
         */
        [[nodiscard]] uint32_t size()   const noexcept;

        /**
         * @brief normalized
         * @return
         */
        [[nodiscard]] bool normalized() const noexcept;

    private:
        friend class InputLayout;

        AttributeType type_ {Undefined};

        std::string tag_ {"Undefined"};

        uint32_t offset_ {0};
        uint32_t count_  {0};
        uint32_t size_   {0};

        bool normalized_ {false};

    };
}
