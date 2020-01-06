#pragma once

#include "event.hpp"

namespace glem::core {

    class State {
    public:
        State() = default;
        virtual ~State() = default;

        /**
         * @brief onAttach
         */
        virtual void onAttach() noexcept = 0;

        /**
         * @brief onDetach
         */
        virtual void onDetach() noexcept = 0;

        /**
         * @brief onEvent
         * @param event
         */
        virtual void onEvent(Event& event) noexcept = 0;

        /**
         * @brief onUpdate
         * @param dt
         */
        virtual void onUpdate(float dt) noexcept = 0;

        /**
         * @brief onDraw
         */
        virtual void onDraw() noexcept = 0;

    };

}
