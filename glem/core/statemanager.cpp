#include "state.hpp"
#include "statemanager.hpp"

#include <algorithm>

namespace glem::core {

    std::deque<std::shared_ptr<State>> StateManager::states_;

    void StateManager::push(const std::shared_ptr<State> &value) noexcept
    {
        value->onAttach();

        states_.emplace_front(value);
    }

    void StateManager::pop() noexcept
    {
        top()->onDetach();

        states_.pop_front();
    }

    void StateManager::erase(const std::shared_ptr<State> &value) noexcept
    {
        if(auto it = std::find(states_.begin(), states_.end(), value); it != states_.end()) {
            (*it)->onDetach();

            states_.erase(it);
        }
    }

    std::shared_ptr<State> StateManager::top() noexcept
    {
        return states_.front();
    }

    bool StateManager::empty() noexcept
    {
        return states_.empty();
    }

}
