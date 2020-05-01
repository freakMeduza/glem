#pragma once

#include "Vertex.hpp"

#include <glm/glm.hpp>

namespace glem {

    struct IndexedTriangleList {
        ~IndexedTriangleList() = default;

        IndexedTriangleList(IndexedTriangleList&&) = default;
        IndexedTriangleList(const IndexedTriangleList&) = default;

        IndexedTriangleList& operator=(IndexedTriangleList&&) = default;
        IndexedTriangleList& operator=(const IndexedTriangleList&) = default;

        IndexedTriangleList(const VertexByteBuffer& b, const std::vector<uint32_t>& i);

        /**
         * @brief Vertex byte buffer
         */
        VertexByteBuffer buffer;

        /**
         * @brief Indices
         */
        std::vector<uint32_t> indices;

        /**
         * @brief Set flat normals
         */
        void setFlat() noexcept;

        /**
         * @brief Transform vertices
         * @param value
         */
        void transform(const glm::mat4& value) noexcept;

    };

    struct Shape {
        Shape() = delete;
        ~Shape() = delete;

        Shape(Shape&&) = delete;
        Shape(const Shape&) = delete;

        Shape& operator=(Shape&&) = delete;
        Shape& operator=(const Shape&) = delete;

        /**
         * @brief Get cube indexed triangle list
         * @return
         */
        static IndexedTriangleList cube() noexcept;

        /**
         * @brief Get cube indexed triangle list with uv
         * @return
         */
        static IndexedTriangleList texturedCube() noexcept;

        /**
         * @brief Get sphere indexed triangle list
         * @param dimension
         * @return
         */
        static IndexedTriangleList sphere(const glm::ivec2& dimension) noexcept;
    };

}

