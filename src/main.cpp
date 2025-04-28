#include "Game.hpp"
#include <iostream>
#include <version>

int main(int argc, char* argv[]) {
    static_assert(__cplusplus >= 202302L, "Le projet n'est pas compilé en C++23 !");
    Game game;
    game.run();
    return 0;
}
