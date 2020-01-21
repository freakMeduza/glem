#include "game.hpp"
#include "level.hpp"
#include "gamestate.hpp"

int main() {
    auto&& game = breakout::Game::instance();

    game.push(std::make_shared<breakout::GameState>());

    return game.exec();
}
