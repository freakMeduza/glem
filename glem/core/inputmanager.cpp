#include "inputmanager.hpp"

namespace {
    const uint8_t MAX_QUEUE_SIZE = 16;

    template<typename T>
    void trim(std::queue<T>& queue) noexcept {
        while(queue.size() > MAX_QUEUE_SIZE)
            queue.pop();
    }
}

namespace glem::core {

    std::queue<MouseEvent>    InputManager::mouse_;
    std::queue<KeyboardEvent> InputManager::keyboard_;

    std::optional<MouseEvent> InputManager::fetchMouseEvent() noexcept
    {
        if(mouse_.empty())
            return {};

        const auto& event = mouse_.front();

        mouse_.pop();

        return event;
    }

    std::optional<KeyboardEvent> InputManager::fetchKeyboardEvent() noexcept
    {
        if(keyboard_.empty())
            return {};

        const auto& event = keyboard_.front();

        keyboard_.pop();

        return event;
    }

    void InputManager::onMouseEvent(const MouseEvent &value) noexcept
    {
        mouse_.push(value);

        trim(mouse_);
    }

    void InputManager::onKeyboardEvent(const KeyboardEvent &value) noexcept
    {
        keyboard_.push(value);

        trim(keyboard_);
    }

}
