#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <SDL.h>
#include "ColorUtils.hpp"
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
    // Init size of window size
    const int windowWidth_ = 500;
    const int windowHeight_ = 500;

    // Map size and pos (in px)
    SDL_Rect mapSize_ = {0, 0, windowWidth_, windowHeight_};
    SDL_Point mapPos_ = {windowWidth_ / 2, windowHeight_ / 2};

    // Grid size
    std::pair<int, int> gridSize_ = {6, 6};
    
    // Init radius of hexagones
    const double hexagonRadius_ = 40.0;

    // Loop condition of game
    bool loop_ = true;

    // Background color
    const SDL_Color bgColor = CLR_SEABLUE;


    // Object to render drawing
    SDL_Renderer* renderer_;

    // Window
    std::unique_ptr<Window> window_;

    // Map of game
    std::optional<GameMap> map_;
};

#endif
