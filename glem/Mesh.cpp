#include "Mesh.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#define PI static_cast<float>(M_PI)

namespace glem {

    IndexedTriangleList::IndexedTriangleList(const VertexByteBuffer &b, const std::vector<uint32_t> &i) :
        buffer {b}, indices {i}
    {

    }

    void IndexedTriangleList::setFlat() noexcept
    {
        for(size_t i = 0; i < indices.size(); i += 3) {
            auto v0 = buffer[indices[  i  ]];
            auto v1 = buffer[indices[i + 1]];
            auto v2 = buffer[indices[i + 2]];

            const auto p0 = v0.attribute<AttributeType::Vector3f>("position");
            const auto p1 = v1.attribute<AttributeType::Vector3f>("position");
            const auto p2 = v2.attribute<AttributeType::Vector3f>("position");

            const auto normal = glm::normalize(glm::cross((p1 - p0), (p2 - p0)));

            v0.attribute<AttributeType::Vector3f>("normal") = normal;
            v1.attribute<AttributeType::Vector3f>("normal") = normal;
            v2.attribute<AttributeType::Vector3f>("normal") = normal;
        }
    }

    void IndexedTriangleList::transform(const glm::mat4 &value) noexcept
    {
        for(size_t i = 0; i < buffer.count(); ++i) {
            auto& position = buffer[i].attribute<AttributeType::Vector3f>("position");

            const auto t = value * glm::vec4{position, 0.0f};

            position = {t.x, t.y, t.z};
        }
    }

    IndexedTriangleList Shape::cube() noexcept
    {
        constexpr float side = 1.0f;

        std::vector<glm::vec3> position(24);

        position[0]  = { -side, -side, -side };
        position[1]  = {  side, -side, -side };
        position[2]  = { -side,  side, -side };
        position[3]  = {  side,  side, -side };
        position[4]  = { -side, -side,  side };
        position[5]  = {  side, -side,  side };
        position[6]  = { -side,  side,  side };
        position[7]  = {  side,  side,  side };
        position[8]  = { -side, -side, -side };
        position[9]  = { -side,  side, -side };
        position[10] = { -side, -side,  side };
        position[11] = { -side,  side,  side };
        position[12] = {  side, -side, -side };
        position[13] = {  side,  side, -side };
        position[14] = {  side, -side,  side };
        position[15] = {  side,  side,  side };
        position[16] = { -side, -side, -side };
        position[17] = {  side, -side, -side };
        position[18] = { -side, -side,  side };
        position[19] = {  side, -side,  side };
        position[20] = { -side,  side, -side };
        position[21] = {  side,  side, -side };
        position[22] = { -side,  side,  side };
        position[23] = {  side,  side,  side };

        VertexByteBuffer buffer{
            VertexLayout{}.push(AttributeType::Vector3f, "position")
                          .push(AttributeType::Vector3f, "normal")
        };

        for(size_t i = 0; i < position.size(); ++i)
            buffer.emplace_back(position[i], glm::vec3{0.0f, 0.0f, 0.0f});

        std::vector<uint32_t> indices {
            0,  2,  1,   2,  3,  1,
            4,  5,  7,   4,  7,  6,
            8, 10,  9,  10, 11,  9,
            12, 13, 15, 12, 15, 14,
            16, 17, 18, 18, 17, 19,
            20, 23, 21, 20, 22, 23
        };

        return {std::move(buffer), std::move(indices)};
    }

    IndexedTriangleList Shape::texturedCube() noexcept
    {
        constexpr float side = 1.0f;

        std::vector<glm::vec3> position(24);

        position[0]  = { -side, -side, -side };
        position[1]  = {  side, -side, -side };
        position[2]  = { -side,  side, -side };
        position[3]  = {  side,  side, -side };
        position[4]  = { -side, -side,  side };
        position[5]  = {  side, -side,  side };
        position[6]  = { -side,  side,  side };
        position[7]  = {  side,  side,  side };
        position[8]  = { -side, -side, -side };
        position[9]  = { -side,  side, -side };
        position[10] = { -side, -side,  side };
        position[11] = { -side,  side,  side };
        position[12] = {  side, -side, -side };
        position[13] = {  side,  side, -side };
        position[14] = {  side, -side,  side };
        position[15] = {  side,  side,  side };
        position[16] = { -side, -side, -side };
        position[17] = {  side, -side, -side };
        position[18] = { -side, -side,  side };
        position[19] = {  side, -side,  side };
        position[20] = { -side,  side, -side };
        position[21] = {  side,  side, -side };
        position[22] = { -side,  side,  side };
        position[23] = {  side,  side,  side };

        std::vector<glm::vec2> uv(24);

        uv[0]  = { 0.0f, 0.0f };
        uv[1]  = { 1.0f, 0.0f };
        uv[2]  = { 0.0f, 1.0f };
        uv[3]  = { 1.0f, 1.0f };
        uv[4]  = { 0.0f, 0.0f };
        uv[5]  = { 1.0f, 0.0f };
        uv[6]  = { 0.0f, 1.0f };
        uv[7]  = { 1.0f, 1.0f };
        uv[8]  = { 0.0f, 0.0f };
        uv[9]  = { 1.0f, 0.0f };
        uv[10] = { 0.0f, 1.0f };
        uv[11] = { 1.0f, 1.0f };
        uv[12] = { 0.0f, 0.0f };
        uv[13] = { 1.0f, 0.0f };
        uv[14] = { 0.0f, 1.0f };
        uv[15] = { 1.0f, 1.0f };
        uv[16] = { 0.0f, 0.0f };
        uv[17] = { 1.0f, 0.0f };
        uv[18] = { 0.0f, 1.0f };
        uv[19] = { 1.0f, 1.0f };
        uv[20] = { 0.0f, 0.0f };
        uv[21] = { 1.0f, 0.0f };
        uv[22] = { 0.0f, 1.0f };
        uv[23] = { 1.0f, 1.0f };

        VertexByteBuffer buffer{
            VertexLayout{}.push(AttributeType::Vector3f, "position")
                          .push(AttributeType::Vector3f, "normal")
                          .push(AttributeType::Vector2f, "uv")
        };

        for(size_t i = 0; i < position.size(); ++i)
            buffer.emplace_back(position[i], glm::vec3{0.0f, 0.0f, 0.0f}, uv[i]);

        std::vector<uint32_t> indices {
            0,  2,  1,   2,  3,  1,
            4,  5,  7,   4,  7,  6,
            8, 10,  9,  10, 11,  9,
            12, 13, 15, 12, 15, 14,
            16, 17, 18, 18, 17, 19,
            20, 23, 21, 20, 22, 23
        };

        return {std::move(buffer), std::move(indices)};
    }

    IndexedTriangleList Shape::sphere(const glm::ivec2 &dimension) noexcept
    {
        std::vector<glm::vec3> position;

        for(int y = 0; y <= dimension.y; ++y) {
            for(int x = 0; x <= dimension.x; ++x) {
                glm::vec2 segment{static_cast<float>(x) / static_cast<float>(dimension.x), static_cast<float>(y) / static_cast<float>(dimension.y)};

                glm::vec3 p{std::cos(segment.x * 2.0f * PI) * std::sin(segment.y * PI),
                            std::cos(segment.y * PI),
                            std::sin(segment.x * 2.0f * PI) * std::sin(segment.y * PI)};

                position.emplace_back(p);
            }
        }

        std::vector<uint32_t> indices;

        bool odd {false};

        for(int y = 0; y < dimension.y; ++y) {
            if(!odd) {
                for(int x = 0; x <= dimension.x; ++x) {
                    indices.emplace_back(   y    * (dimension.x + 1) + x);
                    indices.emplace_back((y + 1) * (dimension.x + 1) + x);
                }
            }
            else {
                for(int x = dimension.x; x >= 0; --x) {
                    indices.emplace_back((y + 1) * (dimension.x + 1) + x);
                    indices.emplace_back(   y    * (dimension.x + 1) + x);
                }
            }

            odd = !odd;
        }

        VertexByteBuffer buffer{
            VertexLayout{}.push(AttributeType::Vector3f, "position")
                          .push(AttributeType::Vector3f, "normal")
        };

        for(size_t i = 0; i < position.size(); ++i)
            buffer.emplace_back(position[i], position[i]);

        return {std::move(buffer), std::move(indices)};
    }

}
