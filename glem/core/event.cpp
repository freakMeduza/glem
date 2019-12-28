#include "event.hpp"

namespace glem::core {

    MouseEvent::MouseEvent(Action action, int x, int y, float scroll, uint16_t keycode) :
        Event    {action},
        x_       {x},
        y_       {y},
        scroll_  {scroll},
        keycode_ {keycode}
    {

    }

    int MouseEvent::x() const noexcept
    {
        return x_;
    }

    int MouseEvent::y() const noexcept
    {
        return y_;
    }

    float MouseEvent::scroll() const noexcept
    {
        return scroll_;
    }

    uint16_t MouseEvent::keycode() const noexcept
    {
        return keycode_;
    }

    KeyboardEvent::KeyboardEvent(Action action, uint16_t keycode) :
        Event    {action},
        keycode_ {keycode}
    {

    }

    uint16_t KeyboardEvent::keycode() const noexcept
    {
        return keycode_;
    }

    Action Event::action() const noexcept
    {
        return action_;
    }

}
