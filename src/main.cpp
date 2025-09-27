#include <iostream>

#include "Game.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
    Game game;
    
    if(game.init(WINDOW_WIDTH, WINDOW_HEIGHT))
        game.run();

    return 0;
}
