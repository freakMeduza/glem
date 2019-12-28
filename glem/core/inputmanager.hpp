#pragma once

#include "event.hpp"

#include <queue>
#include <optional>

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
         * @brief fetchMouseEvent
         * @return
         */
        static std::optional<MouseEvent>    fetchMouseEvent()    noexcept;

        /**
         * @brief fetchKeyboardEvent
         * @return
         */
        static std::optional<KeyboardEvent> fetchKeyboardEvent() noexcept;

    private:
        friend class Window;

        static void onMouseEvent(const MouseEvent& value) noexcept;
        static void onKeyboardEvent(const KeyboardEvent& value) noexcept;

        static std::queue<MouseEvent>    mouse_;
        static std::queue<KeyboardEvent> keyboard_;

    };

}
