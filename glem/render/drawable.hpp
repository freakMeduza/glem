#pragma once

#include <vector>
#include <memory>

#include "texture.hpp"

#include <glm/glm.hpp>

namespace glem::render {

    class Texture;

    class Drawable {
    public:
        Drawable() = default;
        ~Drawable() = default;

        Drawable(const glm::vec3& position,
                 const glm::vec2& size,
                 const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f},
                 const std::shared_ptr<Texture>& texture = nullptr,
                 uint32_t atlasIndex = 0);

        Drawable(Drawable&&) = default;
        Drawable(const Drawable&) = default;

        Drawable& operator=(Drawable&&) = default;
        Drawable& operator=(const Drawable&) = default;

        /**
         * @brief setPosition
         * @param value
         */
        void setPosition(const glm::vec3& value) noexcept;

        /**
         * @brief position
         * @return
         */
        [[nodiscard]] const glm::vec3& position() const noexcept;

        /**
         * @brief setColor
         * @param value
         */
        void setColor(const glm::vec4& value) noexcept;

        /**
         * @brief color
         * @return
         */
        [[nodiscard]] const glm::vec4& color() const noexcept;

        /**
         * @brief setSize
         * @param value
         */
        void setSize(const glm::vec2& value) noexcept;

        /**
         * @brief size
         * @return
         */
        [[nodiscard]] const glm::vec2& size() const noexcept;

        /**
         * @brief setUV
         * @param value
         */
        void setUV(const std::vector<glm::vec2>& value) noexcept;

        /**
         * @brief uv
         * @return
         */
        [[nodiscard]] const std::vector<glm::vec2>& uv() const noexcept;

        /**
         * @brief setTexture
         * @param value
         */
        void setTexture(const std::shared_ptr<Texture>& value) noexcept;

        /**
         * @brief texture
         * @return
         */
        [[nodiscard]] std::shared_ptr<Texture> texture() const noexcept;

        /**
         * @brief setAtlasIndex
         * @param value
         */
        void setAtlasIndex(uint32_t value) noexcept;

        /**
         * @brief atlasIndex
         * @return
         */
        [[nodiscard]] uint32_t atlasIndex() const noexcept;

//        inline void setIndex(uint32_t value) noexcept { index_ = value; }
//        inline uint32_t index() const noexcept { return index_; }

//        inline glm::vec2 offset() const noexcept {
//            int column = index_ % texture_->numberOfRows();
//            int row    = index_ / texture_->numberOfRows();

//            float xoffset = static_cast<float>(column) / static_cast<float>(texture_->numberOfRows());
//            float yoffset = static_cast<float>(row)    / static_cast<float>(texture_->numberOfRows());

//            return {xoffset, yoffset};
//        }

    private:
        void update() noexcept;

        glm::vec3 position_;
        glm::vec4 color_;
        glm::vec2 size_;

        std::vector<glm::vec2> uv_;

        std::shared_ptr<Texture> texture_ {nullptr};

        uint32_t atlasIndex_ {0};
    };

}
