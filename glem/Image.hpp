#pragma once

#include <string>
#include <vector>

namespace glem {

    struct Image {
        Image(int w, int h, const std::vector<uint8_t>& p);
        ~Image() = default;

        static Image load(const std::string& filepath) noexcept;

        int width  {0};
        int heigth {0};

        std::vector<uint8_t> pixels;

    };

}
