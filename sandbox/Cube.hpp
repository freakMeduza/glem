#pragma once

#include <glem/IndexedTriangleList.hpp>

template<typename T>
struct Cube {
    static glem::IndexedTriangleList<T> create() noexcept {
        constexpr float side = 1.0f / 2.0f;

        std::vector<T> vertices(24);
        vertices[0].position  = { -side, -side, -side };
        vertices[1].position  = {  side, -side, -side };
        vertices[2].position  = { -side,  side, -side };
        vertices[3].position  = {  side,  side, -side };
        vertices[4].position  = { -side, -side,  side };
        vertices[5].position  = {  side, -side,  side };
        vertices[6].position  = { -side,  side,  side };
        vertices[7].position  = {  side,  side,  side };
        vertices[8].position  = { -side, -side, -side };
        vertices[9].position  = { -side,  side, -side };
        vertices[10].position = { -side, -side,  side };
        vertices[11].position = { -side,  side,  side };
        vertices[12].position = {  side, -side, -side };
        vertices[13].position = {  side,  side, -side };
        vertices[14].position = {  side, -side,  side };
        vertices[15].position = {  side,  side,  side };
        vertices[16].position = { -side, -side, -side };
        vertices[17].position = {  side, -side, -side };
        vertices[18].position = { -side, -side,  side };
        vertices[19].position = {  side, -side,  side };
        vertices[20].position = { -side,  side, -side };
        vertices[21].position = {  side,  side, -side };
        vertices[22].position = { -side,  side,  side };
        vertices[23].position = {  side,  side,  side };

        std::vector<uint32_t> indices {
             0,  2,  1,  2,  3,  1,
             4,  5,  7,  4,  7,  6,
             8, 10,  9, 10, 11,  9,
            12, 13, 15, 12, 15, 14,
            16, 17, 18, 18, 17, 19,
            20, 23, 21, 20, 22, 23
        };

        return {std::move(vertices), std::move(indices)};
    }
};
