#pragma once

#include <utility>
#include <stdint.h>

namespace glem::core {

    class Event {
    public:
        enum EventType {
            Undefined,
            KeyPressed,
            KeyReleased,
            MouseButtonPressed,
            MouseButtonReleased,
            MouseMoved,
            MouseScrolled,
            MouseEntered,
            MouseLeft
        };

        Event(EventType type) : type_ {type} {

        }

        virtual ~Event() = default;

        /**
         * @brief type
         * @return
         */
        [[nodiscard]] inline EventType type() const noexcept { return type_; }

    private:
        EventType type_ {Undefined};

    };

    class MouseEvent : public Event {
    public:
        MouseEvent(EventType type, uint16_t button, float x, float y, float xoffset = 0.0f, float yoffset = 0.0f) :
            Event    {type},
            x_       {x},
            y_       {y},
            xoffset_ {xoffset},
            yoffset_ {yoffset},
            button_  {button} {

        }

        ~MouseEvent() override = default;

        /**
         * @brief position
         * @return
         */
        [[nodiscard]] inline std::pair<float, float> position() const noexcept { return {x_, y_}; }

        /**
         * @brief offset
         * @return
         */
        [[nodiscard]] inline std::pair<float, float> offset() const noexcept { return {xoffset_, yoffset_}; }

        /**
         * @brief button
         * @return
         */
        [[nodiscard]] inline uint16_t button() const noexcept { return button_; }

    private:
        float x_ {0.0f};
        float y_ {0.0f};

        float xoffset_ {0.0f};
        float yoffset_ {0.0f};

        uint16_t button_ {0};

    };

    class KeyboardEvent : public Event {
    public:
        KeyboardEvent(EventType type, uint16_t keycode) :
            Event    {type},
            keycode_ {keycode} {

        }

        ~KeyboardEvent() override = default;

        /**
         * @brief keycode
         * @return
         */
        [[nodiscard]] inline uint16_t keycode() const noexcept { return keycode_; }

    private:
        uint16_t keycode_ {0};

    };

}
