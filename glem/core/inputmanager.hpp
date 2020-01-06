#pragma once

#include "mouse.hpp"
#include "keyboard.hpp"

namespace glem::core {

    class InputManager {
    public:
        InputManager() = delete;
        ~InputManager() = delete;

        InputManager(InputManager&&) = delete;
        InputManager(const InputManager&) = delete;

        InputManager& operator=(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;

        /**
         * @brief mouse
         * @return
         */
        static const Mouse& mouse() noexcept;

        /**
         * @brief keyboard
         * @return
         */
        static const Keyboard& keyboard() noexcept;

    private:
        static Mouse    mouse_;
        static Keyboard keyboard_;

    };

}

