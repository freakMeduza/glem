#pragma once

#include <string>
#include <vector>

#include <stdexcept>
#include <exception>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <assert.h>

namespace glem {

    enum class AttributeType {
        Vector2f,
        Vector3f
    };

    template<AttributeType> struct AttributeTypeMap;

    template<> struct AttributeTypeMap<AttributeType::Vector2f> {
        using value_type = glm::vec2;

        static constexpr const GLenum systemType = GL_FLOAT;
        static constexpr const size_t size       = sizeof (GLfloat);
        static constexpr const size_t count      = 2;
    };

    template<> struct AttributeTypeMap<AttributeType::Vector3f> {
        using value_type = glm::vec3;

        static constexpr const GLenum systemType = GL_FLOAT;
        static constexpr const size_t size       = sizeof (GLfloat);
        static constexpr const size_t count      = 3;
    };

    class Attribute {
    public:
        Attribute(AttributeType type, const std::string& semantic, size_t offset);
        ~Attribute() = default;

        /**
         * @brief Attribute type
         * @return
         */
        AttributeType type() const noexcept;

        /**
         * @brief Attribute semantic
         * @return
         */
        std::string semantic() const noexcept;

        /**
         * @brief Attribute size
         * @return
         */
        size_t size() const noexcept;

        /**
         * @brief Attribute count
         * @return
         */
        size_t count() const noexcept;

        /**
         * @brief Attribute offset
         * @return
         */
        size_t offset() const noexcept;

    private:
        AttributeType type_;

        std::string semantic_;

        size_t offset_ {0u};

    };

    class VertexLayout {
    public:
        VertexLayout() = default;
        ~VertexLayout() = default;

        /**
         * @brief Push attribute
         * @param type     - Attribute type
         * @param semantic - Attribute semantic
         * @return
         */
        VertexLayout& push(AttributeType type, const std::string& semantic) noexcept;

        /**
         * @brief Attribute
         * @param value - Attribute semantic
         * @return
         */
        const Attribute& attribute(const std::string& value) const;

        /**
         * @brief Attribute by index
         * @param value - Attribute index
         * @return
         */
        const Attribute& attributeByIndex(size_t value) const;

        /**
         * @brief Layout size
         * @return
         */
        size_t size() const noexcept;

        /**
         * @brief Layout attributes count
         * @return
         */
        size_t count() const noexcept;

        /**
         * @brief Attributes
         * @return
         */
        const std::vector<Attribute>& attributes() const noexcept;

    private:
        size_t size_ {0u};

        std::vector<Attribute> layout_;

    };

    class Vertex {
    public:
        template<AttributeType T>
        auto& attribute(const std::string& value) {
            using attribute_type = typename AttributeTypeMap<T>::value_type;

            auto ptr = data_ + layout_.attribute(value).offset();

            return *reinterpret_cast<attribute_type*>(ptr);
        }

        template<typename T>
        void setAttributeByIndex(size_t index, T&& value) {
            const auto& attribute = layout_.attributeByIndex(index);

            auto ptr = data_ + attribute.offset();

            switch (attribute.type()) {
            case AttributeType::Vector2f:
                setAttribute<AttributeType::Vector2f>(ptr, std::forward<T>(value));
                break;
            case AttributeType::Vector3f:
                setAttribute<AttributeType::Vector3f>(ptr, std::forward<T>(value));
                break;
            }
        }

    private:
        friend class VertexByteBuffer;

        Vertex(uint8_t* data, const VertexLayout& layout);

        template<typename T, typename ... Ts>
        void setAttributeByIndex(size_t index, T&& arg, Ts ... args) {
            setAttributeByIndex(index,     std::forward<T>(arg));
            setAttributeByIndex(index + 1, std::forward<Ts>(args) ...);
        }

        template<AttributeType T, typename U>
        void setAttribute(uint8_t* ptr, U&& value) {
            using attribute_type = typename AttributeTypeMap<T>::value_type;

            if constexpr (std::is_assignable_v<attribute_type, std::decay_t<U>>) {
                *reinterpret_cast<attribute_type*>(ptr) = value;
            }
            else
                throw std::runtime_error("Parameter attribute type mismatch.");
        }

        const VertexLayout& layout_;

        uint8_t* data_ {nullptr};

    };

    class VertexByteBuffer {
    public:
        VertexByteBuffer(const VertexLayout& layout);
        ~VertexByteBuffer() = default;

        inline Vertex operator[](size_t index) {
            if(index > count())
                std::runtime_error{"Stack overflow."};

            return Vertex{ buffer_.data() + layout_.size() * index, layout_ };
        }

        /**
         * @brief Buffer data
         * @return
         */
        const uint8_t* data() const noexcept;

        /**
         * @brief Vertex layout
         * @return
         */
        const VertexLayout& layout() const noexcept;

        template<typename ... Ts>
        void emplace_back(Ts&& ... args) {
            if(sizeof ... (args) != layout_.count())
                throw std::runtime_error("Parameter count doesn't match number of vertex attributes.");

            buffer_.resize(buffer_.size() + layout_.size());

            back().setAttributeByIndex(0u, std::forward<Ts>(args) ...);
        }

        /**
         * @brief Buffer size
         * @return
         */
        size_t size() const noexcept;

        /**
         * @brief Vertices count
         * @return
         */
        size_t count() const noexcept;

        /**
         * @brief Front
         * @return
         */
        Vertex front() const;

        /**
         * @brief Back
         * @return
         */
        Vertex back() const;

    private:
        VertexLayout layout_;

        std::vector<uint8_t> buffer_;

    };

}
