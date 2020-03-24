#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glem {

    class Camera {
    public:
        virtual ~Camera() = default;

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
        glm::vec3 position_ {glm::vec3{0.0f}};

        glm::mat4 projection_ {glm::mat4{1.0f}};
        glm::mat4 view_       {glm::mat4{1.0f}};

    };

    class FreeCamera : public Camera {
    public:
        FreeCamera();
        ~FreeCamera() override;

        // Camera interface
        void update(float deltaTime) noexcept override;

    private:
        float sensitivity_ {0.0f};

        float speed_  {0.0f};
        float sprint_ {0.0f};

        float pitch_   {0.0f};
        float yaw_     {0.0f};

        glm::vec2 mouse_ {glm::vec2{0.0f}};

    };

    class MayaCamera : public Camera {
    public:
        MayaCamera();
        ~MayaCamera() override;

        // Camera interface
        void update(float deltaTime) noexcept override;

    private:
        float sensitivity_ {0.0f};

        float pitch_   {0.0f};
        float yaw_     {0.0f};

        glm::vec2 mouse_ {glm::vec2{0.0f}};

    };

}
