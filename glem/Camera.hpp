#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glem {

    enum Movement {
        Forward,
        Backward,
        Left,
        Right
    };

    class OldCamera {
    public:
        OldCamera();
        ~OldCamera();

        OldCamera(const glm::vec3& pos,
               const glm::vec3& wu = glm::vec3{0.0f, 1.0f, 0.0f},
               float y = -90.0f,
               float p = 0.0f);

        glm::mat4 viewMatrix() const noexcept;

        void processMouse(float x_offset, float y_offset) noexcept;
        void processKeyboard(Movement movement, float dt) noexcept;

        glm::vec3 position {glm::vec3{0.0f}};
        glm::vec3 front    {glm::vec3{0.0f}};
        glm::vec3 right    {glm::vec3{0.0f}};
        glm::vec3 up       {glm::vec3{0.0f}};

        glm::vec3 worldUp  {glm::vec3{0.0f}};

        float yaw   {0.0f};
        float pitch {0.0f};

        float speed       {3.0f};
        float sensitivity {0.05f};

    private:
        void update() noexcept;
    };



















    class Camera {
    public:
        virtual ~Camera() = default;

        /**
         * @brief Focus camera
         */
        virtual void focus() noexcept {}

        /**
         * @brief Update camera
         * @param deltaTime - delta time
         */
        virtual void update(float deltaTime) noexcept { static_cast<void>(deltaTime); }

        /**
         * @brief Set camera position
         * @param value
         */
        inline void setPosition(const glm::vec3& value) noexcept {
            position_ = value;
        }

        /**
         * @brief Position
         * @return
         */
        inline glm::vec3 position() const noexcept {
            return position_;
        }

        /**
         * @brief Set focal point
         * @param value
         */
        inline void setLookAt(const glm::vec3& value) noexcept {
            lookAt_ = value;
        }

        /**
         * @brief Focal point
         * @return
         */
        inline glm::vec3 lookAt() const noexcept {
            return lookAt_;
        }

        /**
         * @brief Set projection matrix
         * @param value
         */
        inline void setProjection(const glm::mat4& value) noexcept {
            projection_ = value;
        }

        /**
         * @brief Projection matrix
         * @return
         */
        inline glm::mat4 projection() const noexcept {
            return projection_;
        }

        /**
         * @brief View matrix
         * @return
         */
        inline glm::mat4 view() const noexcept {
            return view_;
        }

    protected:
        glm::vec3 position_  {glm::vec3{0.0f}};
        glm::vec3 lookAt_    {glm::vec3{0.0f}};

        glm::mat4 projection_ {glm::mat4{1.0f}};
        glm::mat4 view_       {glm::mat4{1.0f}};

    };

}
