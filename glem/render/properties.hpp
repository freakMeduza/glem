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

    struct Properties {
        Wrap   wrap      {Wrap::Unspecified};
        Filter minFilter {Filter::Unspecified};
        Filter magFilter {Filter::Unspecified};
        Format format    {Format::Unspecified};

        Properties() :
            wrap      {Wrap::Repeat},
            minFilter {Filter::Linear},
            magFilter {Filter::Linear},
            format    {Format::RGBA}
        {

        }
    };

    struct Options {
        bool verticalFlip {false};

        Options() :
            verticalFlip {false}
        {

        }
    };

}
