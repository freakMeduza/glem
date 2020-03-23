#pragma once

#include <vector>
#include <stdint.h>

#include <glm/glm.hpp>

namespace primitives {

    template<typename T>
    struct IndexedTriangleList {
        IndexedTriangleList() = default;
        ~IndexedTriangleList() = default;

        IndexedTriangleList(const std::vector<T>& v, const std::vector<uint32_t>& i) :
            vertices{v}, indices{i}
        {

        }

        void setNormal() noexcept {
            assert(indices.size() % 3 == 0 && indices.size() > 0);

            for(uint32_t i = 0; i < indices.size(); i += 3) {
                auto& v0 = vertices[indices[  i  ]];
                auto& v1 = vertices[indices[i + 1]];
                auto& v2 = vertices[indices[i + 2]];

                const auto p0 = v0.position;
                const auto p1 = v1.position;
                const auto p2 = v2.position;

                auto normal = glm::normalize(glm::cross((p1 - p0), (p2 - p0)));

                v0.normal = normal;
                v1.normal = normal;
                v2.normal = normal;
            }
        }

        void transform(const glm::mat4& value) noexcept {
            for(auto& v : vertices) {
                auto t = value * glm::vec4(v.position, 0.0f);

                v.position = {t.x, t.y, t.z};
            }
        }

        std::vector<T>        vertices;
        std::vector<uint32_t> indices;
    };

}
