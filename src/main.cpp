#include "Game.hpp"

int main() {
    Game game;
    
    while (game.IsOpen()) {
        game.Update();
        game.Render();
    }
    return 0;
}
