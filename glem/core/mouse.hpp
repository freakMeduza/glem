#pragma once

#include <glm/glm.hpp>

namespace glem::core {

    class Mouse {
    public:
        enum Button {
            Unknown                = -1,

            // From glfw3.h
            Button0                = 0,
            Button1                = 1,
            Button2                = 2,
            Button3                = 3,
            Button4                = 4,
            Button5                = 5,
            Button6                = 6,
            Button7                = 7,

            ButtonLast             = Button7,
            ButtonLeft             = Button0,
            ButtonRight            = Button1,
            ButtonMiddle           = Button2
        };

        Mouse() = default;
        ~Mouse() = default;

        Mouse(Mouse&&) = delete;
        Mouse(const Mouse&) = delete;

        Mouse& operator=(Mouse&&) = delete;
        Mouse& operator=(const Mouse&) = delete;

        /**
         * @brief isButtonPressed
         * @param button
         * @return
         */
        bool isButtonPressed(Button button) const noexcept;

        /**
         * @brief position
         * @return
         */
        glm::vec2 position() const noexcept;

    };

}
