#pragma once

#include <glm/glm.hpp>

#include "Input.hpp"

namespace glem {

    class Mouse : public Input {
    public:
        enum class Button {
            // From glfw3.h
            Button0      = 0,
            Button1      = 1,
            Button2      = 2,
            Button3      = 3,
            Button4      = 4,
            Button5      = 5,
            Button6      = 6,
            Button7      = 7,

            ButtonLast   = Button7,
            ButtonLeft   = Button0,
            ButtonRight  = Button1,
            ButtonMiddle = Button2
        };

        Mouse() = delete;
        ~Mouse() = delete;

        Mouse(Mouse&&) = delete;
        Mouse(const Mouse&) = delete;

        Mouse& operator=(Mouse&&) = delete;
        Mouse& operator=(const Mouse&) = delete;

        /**
         * @brief Is button pressed
         * @param value - button
         * @return
         */
        static bool isButtonPressed(Button value) noexcept;

        /**
         * @brief Mouse position
         * @return
         */
        static glm::vec2 position() noexcept;

    };

}
