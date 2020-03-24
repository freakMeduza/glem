#pragma once

#include <glm/glm.hpp>

#include "IndexedTriangleList.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#define PI static_cast<float>(M_PI)

namespace primitives {

    template<typename T>
    struct Sphere {
        static IndexedTriangleList<T> create(const glm::ivec2& dimension) noexcept {
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

            std::vector<T> vertices(position.size());

            for(size_t i = 0; i < vertices.size(); ++i)
                vertices[i].position = position[i];

            return {std::move(vertices), std::move(indices)};
        }
    };

}
