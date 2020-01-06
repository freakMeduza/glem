#pragma once

#include <deque>
#include <memory>

namespace glem::core {

    class State;

    class StateManager {
    public:
        StateManager() = delete;
        ~StateManager() = delete;

        StateManager(StateManager&&) = delete;
        StateManager(const StateManager&) = delete;

        StateManager& operator=(StateManager&&) = delete;
        StateManager& operator=(const StateManager&) = delete;

        /**
         * @brief push
         * @param value
         */
        static void push(const std::shared_ptr<State>& value) noexcept;

        /**
         * @brief pop
         */
        static void pop() noexcept;

        /**
         * @brief pop
         * @param value
         */
        static void erase(const std::shared_ptr<State>& value) noexcept;

        /**
         * @brief top
         * @return
         */
        static std::shared_ptr<State> top() noexcept;

        /**
         * @brief empty
         * @return
         */
        static bool empty() noexcept;

    private:
        static std::deque<std::shared_ptr<State>> states_;

    };

}
