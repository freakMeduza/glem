#pragma once

#include <string>
#include <vector>
#include <optional>

namespace glem {

    class Image {
    public:
        Image() = default;
        ~Image() = default;

        Image(int width, int height, int channels, const std::vector<uint8_t>& data);

        Image(Image&&) = default;
        Image(const Image&) = default;

        Image& operator=(Image&&) = default;
        Image& operator=(const Image&) = default;

        /**
         * @brief Load image
         * @param filepath - image path
         * @param flip     - vertical flip flag
         * @return
         */
        static std::optional<Image> load(const std::string& filepath, bool flip = true) noexcept;

        /**
         * @brief Save image
         * @param image    - Image
         * @param filepath - filepath
         * @return
         */
        static bool save(const Image& value, const std::string& filepath) noexcept;

        /**
         * @brief Image width
         * @return
         */
        int width() const noexcept;

        /**
         * @brief Image height
         * @return
         */
        int height() const noexcept;

        /**
         * @brief Image channels
         * @return
         */
        int channels() const noexcept;

        /**
         * @brief Image data
         * @return
         */
        const std::vector<uint8_t>& data() const noexcept;

    private:
        int width_    {0};
        int height_   {0};
        int channels_ {0};

        std::vector<uint8_t> data_;

    };

}
