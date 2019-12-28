#pragma once

#include <stdint.h>

namespace glem::core {

    enum Action {
        Undefined,
        Moved,
        Pressed,
        Released,
        Scrolled
    };

    class Event {
    public:
        Event(Action action) :
            action_ {action} {

        }

        virtual ~Event() = default;

        /**
         * @brief action
         * @return
         */
        Action action() const noexcept;

    private:
        Action action_ {Action::Undefined};

    };

    class MouseEvent final :  public Event {
    public:
        MouseEvent(Action action, int x, int y, float scroll, uint16_t keycode);
        ~MouseEvent() override = default;

        /**
         * @brief x
         * @return
         */
        int x() const noexcept;

        /**
         * @brief y
         * @return
         */
        int y() const noexcept;

        /**
         * @brief scroll
         * @return
         */
        float scroll() const noexcept;

        /**
         * @brief keycode
         * @return
         */
        uint16_t keycode() const noexcept;

    private:
        int x_ {0};
        int y_ {0};

        float scroll_ {0.0f};

        uint16_t keycode_ {0};

    };

    class KeyboardEvent final : public Event {
    public:
        KeyboardEvent(Action action, uint16_t keycode);
        ~KeyboardEvent() override = default;

        /**
         * @brief keycode
         * @return
         */
        uint16_t keycode() const noexcept;

    private:
        uint16_t keycode_ {0};

    };

    enum codes
    {
        // From glfw3.h
        Space               = 32,
        Apostrophe          = 39, /* ' */
        Comma               = 44, /* , */
        Minus               = 45, /* - */
        Period              = 46, /* . */
        Slash               = 47, /* / */

        D0                  = 48, /* 0 */
        D1                  = 49, /* 1 */
        D2                  = 50, /* 2 */
        D3                  = 51, /* 3 */
        D4                  = 52, /* 4 */
        D5                  = 53, /* 5 */
        D6                  = 54, /* 6 */
        D7                  = 55, /* 7 */
        D8                  = 56, /* 8 */
        D9                  = 57, /* 9 */

        Semicolon           = 59, /* ; */
        Equal               = 61, /* = */

        A                   = 65,
        B                   = 66,
        C                   = 67,
        D                   = 68,
        E                   = 69,
        F                   = 70,
        G                   = 71,
        H                   = 72,
        I                   = 73,
        J                   = 74,
        K                   = 75,
        L                   = 76,
        M                   = 77,
        N                   = 78,
        O                   = 79,
        P                   = 80,
        Q                   = 81,
        R                   = 82,
        S                   = 83,
        T                   = 84,
        U                   = 85,
        V                   = 86,
        W                   = 87,
        X                   = 88,
        Y                   = 89,
        Z                   = 90,

        LeftBracket         = 91,  /* [ */
        Backslash           = 92,  /* \ */
        RightBracket        = 93,  /* ] */
        GraveAccent         = 96,  /* ` */

        World1              = 161, /* non-US #1 */
        World2              = 162, /* non-US #2 */

        /* Function keys */
        Escape              = 256,
        Enter               = 257,
        Tab                 = 258,
        Backspace           = 259,
        Insert              = 260,
        Delete              = 261,
        Right               = 262,
        Left                = 263,
        Down                = 264,
        Up                  = 265,
        PageUp              = 266,
        PageDown            = 267,
        Home                = 268,
        End                 = 269,
        CapsLock            = 280,
        ScrollLock          = 281,
        NumLock             = 282,
        PrintScreen         = 283,
        Pause               = 284,
        F1                  = 290,
        F2                  = 291,
        F3                  = 292,
        F4                  = 293,
        F5                  = 294,
        F6                  = 295,
        F7                  = 296,
        F8                  = 297,
        F9                  = 298,
        F10                 = 299,
        F11                 = 300,
        F12                 = 301,
        F13                 = 302,
        F14                 = 303,
        F15                 = 304,
        F16                 = 305,
        F17                 = 306,
        F18                 = 307,
        F19                 = 308,
        F20                 = 309,
        F21                 = 310,
        F22                 = 311,
        F23                 = 312,
        F24                 = 313,
        F25                 = 314,

        /* Keypad */
        KP0                 = 320,
        KP1                 = 321,
        KP2                 = 322,
        KP3                 = 323,
        KP4                 = 324,
        KP5                 = 325,
        KP6                 = 326,
        KP7                 = 327,
        KP8                 = 328,
        KP9                 = 329,
        KPDecimal           = 330,
        KPDivide            = 331,
        KPMultiply          = 332,
        KPSubtract          = 333,
        KPAdd               = 334,
        KPEnter             = 335,
        KPEqual             = 336,

        LeftShift           = 340,
        LeftControl         = 341,
        LeftAlt             = 342,
        LeftSuper           = 343,
        RightShift          = 344,
        RightControl        = 345,
        RightAlt            = 346,
        RightSuper          = 347,
        Menu                = 348
    };

    using key = codes;
}

// From glfw3.h
#define GLEM_KEY_SPACE           static_cast<uint16_t>(::glem::core::key::Space)
#define GLEM_KEY_APOSTROPHE      static_cast<uint16_t>(::glem::core::key::Apostrophe)    /* ' */
#define GLEM_KEY_COMMA           static_cast<uint16_t>(::glem::core::key::Comma)         /* , */
#define GLEM_KEY_MINUS           static_cast<uint16_t>(::glem::core::key::Minus)         /* - */
#define GLEM_KEY_PERIOD          static_cast<uint16_t>(::glem::core::key::Period)        /* . */
#define GLEM_KEY_SLASH           static_cast<uint16_t>(::glem::core::key::Slash)         /* / */
#define GLEM_KEY_0               static_cast<uint16_t>(::glem::core::key::D0)
#define GLEM_KEY_1               static_cast<uint16_t>(::glem::core::key::D1)
#define GLEM_KEY_2               static_cast<uint16_t>(::glem::core::key::D2)
#define GLEM_KEY_3               static_cast<uint16_t>(::glem::core::key::D3)
#define GLEM_KEY_4               static_cast<uint16_t>(::glem::core::key::D4)
#define GLEM_KEY_5               static_cast<uint16_t>(::glem::core::key::D5)
#define GLEM_KEY_6               static_cast<uint16_t>(::glem::core::key::D6)
#define GLEM_KEY_7               static_cast<uint16_t>(::glem::core::key::D7)
#define GLEM_KEY_8               static_cast<uint16_t>(::glem::core::key::D8)
#define GLEM_KEY_9               static_cast<uint16_t>(::glem::core::key::D9)
#define GLEM_KEY_SEMICOLON       static_cast<uint16_t>(::glem::core::key::Semicolon)     /* ; */
#define GLEM_KEY_EQUAL           static_cast<uint16_t>(::glem::core::key::Equal)         /* = */
#define GLEM_KEY_A               static_cast<uint16_t>(::glem::core::key::A)
#define GLEM_KEY_B               static_cast<uint16_t>(::glem::core::key::B)
#define GLEM_KEY_C               static_cast<uint16_t>(::glem::core::key::C)
#define GLEM_KEY_D               static_cast<uint16_t>(::glem::core::key::D)
#define GLEM_KEY_E               static_cast<uint16_t>(::glem::core::key::E)
#define GLEM_KEY_F               static_cast<uint16_t>(::glem::core::key::F)
#define GLEM_KEY_G               static_cast<uint16_t>(::glem::core::key::G)
#define GLEM_KEY_H               static_cast<uint16_t>(::glem::core::key::H)
#define GLEM_KEY_I               static_cast<uint16_t>(::glem::core::key::I)
#define GLEM_KEY_J               static_cast<uint16_t>(::glem::core::key::J)
#define GLEM_KEY_K               static_cast<uint16_t>(::glem::core::key::K)
#define GLEM_KEY_L               static_cast<uint16_t>(::glem::core::key::L)
#define GLEM_KEY_M               static_cast<uint16_t>(::glem::core::key::M)
#define GLEM_KEY_N               static_cast<uint16_t>(::glem::core::key::N)
#define GLEM_KEY_O               static_cast<uint16_t>(::glem::core::key::O)
#define GLEM_KEY_P               static_cast<uint16_t>(::glem::core::key::P)
#define GLEM_KEY_Q               static_cast<uint16_t>(::glem::core::key::Q)
#define GLEM_KEY_R               static_cast<uint16_t>(::glem::core::key::R)
#define GLEM_KEY_S               static_cast<uint16_t>(::glem::core::key::S)
#define GLEM_KEY_T               static_cast<uint16_t>(::glem::core::key::T)
#define GLEM_KEY_U               static_cast<uint16_t>(::glem::core::key::U)
#define GLEM_KEY_V               static_cast<uint16_t>(::glem::core::key::V)
#define GLEM_KEY_W               static_cast<uint16_t>(::glem::core::key::W)
#define GLEM_KEY_X               static_cast<uint16_t>(::glem::core::key::X)
#define GLEM_KEY_Y               static_cast<uint16_t>(::glem::core::key::Y)
#define GLEM_KEY_Z               static_cast<uint16_t>(::glem::core::key::Z)
#define GLEM_KEY_LEFT_BRACKET    static_cast<uint16_t>(::glem::core::key::LeftBracket)   /* [ */
#define GLEM_KEY_BACKSLASH       static_cast<uint16_t>(::glem::core::key::Backslash)     /* \ */
#define GLEM_KEY_RIGHT_BRACKET   static_cast<uint16_t>(::glem::core::key::RightBracket)  /* ] */
#define GLEM_KEY_GRAVE_ACCENT    static_cast<uint16_t>(::glem::core::key::GraveAccent)   /* ` */
#define GLEM_KEY_WORLD_1         static_cast<uint16_t>(::glem::core::key::World1)        /* non-US #1 */
#define GLEM_KEY_WORLD_2         static_cast<uint16_t>(::glem::core::key::World2)        /* non-US #2 */

/* Function keys */
#define GLEM_KEY_ESCAPE          static_cast<uint16_t>(::glem::core::key::Escape)
#define GLEM_KEY_ENTER           static_cast<uint16_t>(::glem::core::key::Enter)
#define GLEM_KEY_TAB             static_cast<uint16_t>(::glem::core::key::Tab)
#define GLEM_KEY_BACKSPACE       static_cast<uint16_t>(::glem::core::key::Backspace)
#define GLEM_KEY_INSERT          static_cast<uint16_t>(::glem::core::key::Insert)
#define GLEM_KEY_DELETE          static_cast<uint16_t>(::glem::core::key::Delete)
#define GLEM_KEY_RIGHT           static_cast<uint16_t>(::glem::core::key::Right)
#define GLEM_KEY_LEFT            static_cast<uint16_t>(::glem::core::key::Left)
#define GLEM_KEY_DOWN            static_cast<uint16_t>(::glem::core::key::Down)
#define GLEM_KEY_UP              static_cast<uint16_t>(::glem::core::key::Up)
#define GLEM_KEY_PAGE_UP         static_cast<uint16_t>(::glem::core::key::PageUp)
#define GLEM_KEY_PAGE_DOWN       static_cast<uint16_t>(::glem::core::key::PageDown)
#define GLEM_KEY_HOME            static_cast<uint16_t>(::glem::core::key::Home)
#define GLEM_KEY_END             static_cast<uint16_t>(::glem::core::key::End)
#define GLEM_KEY_CAPS_LOCK       static_cast<uint16_t>(::glem::core::key::CapsLock)
#define GLEM_KEY_SCROLL_LOCK     static_cast<uint16_t>(::glem::core::key::ScrollLock)
#define GLEM_KEY_NUM_LOCK        static_cast<uint16_t>(::glem::core::key::NumLock)
#define GLEM_KEY_PRINT_SCREEN    static_cast<uint16_t>(::glem::core::key::PrintScreen)
#define GLEM_KEY_PAUSE           static_cast<uint16_t>(::glem::core::key::Pause)
#define GLEM_KEY_F1              static_cast<uint16_t>(::glem::core::key::F1)
#define GLEM_KEY_F2              static_cast<uint16_t>(::glem::core::key::F2)
#define GLEM_KEY_F3              static_cast<uint16_t>(::glem::core::key::F3)
#define GLEM_KEY_F4              static_cast<uint16_t>(::glem::core::key::F4)
#define GLEM_KEY_F5              static_cast<uint16_t>(::glem::core::key::F5)
#define GLEM_KEY_F6              static_cast<uint16_t>(::glem::core::key::F6)
#define GLEM_KEY_F7              static_cast<uint16_t>(::glem::core::key::F7)
#define GLEM_KEY_F8              static_cast<uint16_t>(::glem::core::key::F8)
#define GLEM_KEY_F9              static_cast<uint16_t>(::glem::core::key::F9)
#define GLEM_KEY_F10             static_cast<uint16_t>(::glem::core::key::F10)
#define GLEM_KEY_F11             static_cast<uint16_t>(::glem::core::key::F11)
#define GLEM_KEY_F12             static_cast<uint16_t>(::glem::core::key::F12)
#define GLEM_KEY_F13             static_cast<uint16_t>(::glem::core::key::F13)
#define GLEM_KEY_F14             static_cast<uint16_t>(::glem::core::key::F14)
#define GLEM_KEY_F15             static_cast<uint16_t>(::glem::core::key::F15)
#define GLEM_KEY_F16             static_cast<uint16_t>(::glem::core::key::F16)
#define GLEM_KEY_F17             static_cast<uint16_t>(::glem::core::key::F17)
#define GLEM_KEY_F18             static_cast<uint16_t>(::glem::core::key::F18)
#define GLEM_KEY_F19             static_cast<uint16_t>(::glem::core::key::F19)
#define GLEM_KEY_F20             static_cast<uint16_t>(::glem::core::key::F20)
#define GLEM_KEY_F21             static_cast<uint16_t>(::glem::core::key::F21)
#define GLEM_KEY_F22             static_cast<uint16_t>(::glem::core::key::F22)
#define GLEM_KEY_F23             static_cast<uint16_t>(::glem::core::key::F23)
#define GLEM_KEY_F24             static_cast<uint16_t>(::glem::core::key::F24)
#define GLEM_KEY_F25             static_cast<uint16_t>(::glem::core::key::F25)

/* Keypad */
#define GLEM_KEY_KP_0            static_cast<uint16_t>(::glem::core::key::KP0)
#define GLEM_KEY_KP_1            static_cast<uint16_t>(::glem::core::key::KP1)
#define GLEM_KEY_KP_2            static_cast<uint16_t>(::glem::core::key::KP2)
#define GLEM_KEY_KP_3            static_cast<uint16_t>(::glem::core::key::KP3)
#define GLEM_KEY_KP_4            static_cast<uint16_t>(::glem::core::key::KP4)
#define GLEM_KEY_KP_5            static_cast<uint16_t>(::glem::core::key::KP5)
#define GLEM_KEY_KP_6            static_cast<uint16_t>(::glem::core::key::KP6)
#define GLEM_KEY_KP_7            static_cast<uint16_t>(::glem::core::key::KP7)
#define GLEM_KEY_KP_8            static_cast<uint16_t>(::glem::core::key::KP8)
#define GLEM_KEY_KP_9            static_cast<uint16_t>(::glem::core::key::KP9)
#define GLEM_KEY_KP_DECIMAL      static_cast<uint16_t>(::glem::core::key::KPDecimal)
#define GLEM_KEY_KP_DIVIDE       static_cast<uint16_t>(::glem::core::key::KPDivide)
#define GLEM_KEY_KP_MULTIPLY     static_cast<uint16_t>(::glem::core::key::KPMultiply)
#define GLEM_KEY_KP_SUBTRACT     static_cast<uint16_t>(::glem::core::key::KPSubtract)
#define GLEM_KEY_KP_ADD          static_cast<uint16_t>(::glem::core::key::KPAdd)
#define GLEM_KEY_KP_ENTER        static_cast<uint16_t>(::glem::core::key::KPEnter)
#define GLEM_KEY_KP_EQUAL        static_cast<uint16_t>(::glem::core::key::KPEqual)

#define GLEM_KEY_LEFT_SHIFT      static_cast<uint16_t>(::glem::core::key::LeftShift)
#define GLEM_KEY_LEFT_CONTROL    static_cast<uint16_t>(::glem::core::key::LeftControl)
#define GLEM_KEY_LEFT_ALT        static_cast<uint16_t>(::glem::core::key::LeftAlt)
#define GLEM_KEY_LEFT_SUPER      static_cast<uint16_t>(::glem::core::key::LeftSuper)
#define GLEM_KEY_RIGHT_SHIFT     static_cast<uint16_t>(::glem::core::key::RightShift)
#define GLEM_KEY_RIGHT_CONTROL   static_cast<uint16_t>(::glem::core::key::RightControl)
#define GLEM_KEY_RIGHT_ALT       static_cast<uint16_t>(::glem::core::key::RightAlt)
#define GLEM_KEY_RIGHT_SUPER     static_cast<uint16_t>(::glem::core::key::RightSuper)
#define GLEM_KEY_MENU            static_cast<uint16_t>(::glem::core::key::Menu)
