#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "SDL.h"
#include "ColorUtils.hpp"
#include "GameMap.hpp"
#include "Window.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    ~Game();

    void handleEvents();
    void draw();
    void run();
    

private:
    double hexSize_ = 100;
    bool moved_ = false;

    // Init size of window size
    Size windowSize_ = {500, 500};

    // Map size and pos (in px)
    Point mapPos_ = {0, 0};

    // Grid size
    std::pair<int, int> gridSize_ = {20, 20};

    // Loop condition of game
    bool loop_ = true;

    std::optional<SDL_Point> moveOrigin_;

    // Background color
    const SDL_Color bgColor = ColorUtils::SEABLUE;


    // Object to render drawing
    SDL_Renderer* renderer_;

    // Window
    std::unique_ptr<Window> window_;

    // Map of game
    std::optional<GameMap> map_;

    Player p1_{"Jean", ColorUtils::RED};
    Player p2_{"Théo", ColorUtils::BLUE};
    Player p3_{"Kevin", ColorUtils::LIGHT_GOLDENROD_YELLOW};
};

#endif
