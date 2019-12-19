#pragma once

namespace glem::render {

    enum class Format {
        Unspecified = 0,
        RGB,
        RGBA,
    };

    enum class Filter {
        Unspecified = 0,
        Linear,
        Nearest,
    };

    enum class Wrap {
        Unspecified = 0,
        Repeat,
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat
    };

}
