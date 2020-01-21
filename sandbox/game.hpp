#pragma once

#include <glem.hpp>

namespace breakout  {

    enum State {
        InMenu,
        InGame
    };

    class Game : public glem::core::Application {
    public:
        ~Game() override;

        void push(const std::shared_ptr<glem::core::State>& state) noexcept;
        void pop() noexcept;

    private:

    };

}
