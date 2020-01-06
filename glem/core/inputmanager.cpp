#include "inputmanager.hpp"

namespace glem::core {

    Mouse    InputManager::mouse_;
    Keyboard InputManager::keyboard_;

    const Mouse &InputManager::mouse() noexcept
    {
        return mouse_;
    }

    const Keyboard &InputManager::keyboard() noexcept
    {
        return keyboard_;
    }

}
