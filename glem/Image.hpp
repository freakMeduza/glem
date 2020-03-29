#pragma once

#include <string>
#include <vector>

namespace glem {

    struct Image {
        Image() = default;
        ~Image() = default;

        Image(int w, int h, const std::vector<uint8_t>& p);

        static Image load(const std::string& filepath, bool flip = true) noexcept;

        int width  {0};
        int height {0};

        std::vector<uint8_t> pixels;

    };

}
