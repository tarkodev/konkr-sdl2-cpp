#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <SDL.h>
#include "GameMap.hpp"
#include "Window.hpp"

class Game {
public:
    Game();
    ~Game();

    void handleEvents();
    void draw();
    void run();

private:
    // Object to render drawing
    SDL_Renderer* renderer_;


    // Window
    std::unique_ptr<Window> window_;

    // Init size of window size
    const int windowWidth_ = 500;
    const int windowHeight_ = 500;


    // Map of game
    std::optional<GameMap> map_;

    // Map size and pos (in px)
    SDL_Rect mapSize_ = {0, 0, windowWidth_, windowHeight_};
    SDL_Point mapPos_ = {windowWidth_ / 2, windowHeight_ / 2};

    
    // Init radius of hexagones
    const double hexagonRadius_ = 40.0;

    // Grid size
    std::pair<int, int> gridSize_ = {6, 6};

    // Loop condition of game
    bool loop_ = true;
};

#endif
