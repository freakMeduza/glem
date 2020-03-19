#pragma once

#include "Camera.hpp"

namespace glem {

    class FreeCamera : public Camera {
    public:
        FreeCamera();
        ~FreeCamera() override;

        // Camera interface
        void focus() noexcept override;
        void update() noexcept override;

    private:
        float sensitivity_ {0.0f};

        float speed_  {0.0f};
        float sprint_ {0.0f};

        float pitch_   {0.0f};
        float heading_ {0.0f};

    };

}
