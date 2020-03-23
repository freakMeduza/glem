#pragma once

#include <glm/glm.hpp>

#include "IndexedTriangleList.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

namespace primitives {

    template<typename T>
    struct Sphere {
        static IndexedTriangleList<T> create(const glm::ivec2& dimension, float radius = 1.0f) noexcept {
            const float R = 1.0f / static_cast<float>(dimension.x - 1.0f);
            const float S = 1.0f / static_cast<float>(dimension.y - 1.0f);

            std::vector<T> vertices(dimension.x * dimension.y * 3);

            auto vertex = vertices.begin();

            for(int i = 0; i < dimension.x; ++i) {
                for(int j = 0; j < dimension.y; ++j) {
                    const float y = std::sin(-M_PI_2 + M_PI * i * R);
                    const float x = std::cos(2 * M_PI * j * S) * std::sin(M_PI * i * R);
                    const float z = std::sin(2 * M_PI * j * S) * std::sin(M_PI * i * R);

                    (*vertex).position = {x * radius, y * radius, z * radius};

                    ++vertex;
                }
            }

            std::vector<uint32_t> indices(dimension.x * dimension.y * 4);

            auto index = indices.begin();

            for(int i = 0; i < dimension.x; ++i) {
                for(int j = 0; j < dimension.y; ++j) {
                    *index++ = i * dimension.y + j;
                    *index++ = i * dimension.y + (j + 1);

                    *index++ = (i + 1) * dimension.y + (j + 1);
                    *index++ = (i + 1) * dimension.y + (j);
                }
            }

            return {std::move(vertices), std::move(indices)};
        }
    };

}
