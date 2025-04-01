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
    void run();

private:
    SDL_Renderer* renderer_;

    // Window size
    const int windowWidth_ = 500;
    const int windowHeight_ = 500;

    std::pair<int, int> gridSize_ = std::make_pair(6, 6);
    std::pair<int, int> mapSize_ = std::make_pair(windowWidth_, windowHeight_);
    std::pair<int, int> mapPos_ = std::make_pair(windowWidth_ / 2, windowHeight_ / 2);

    // Rayon des hexagones (modifiable via la roulette)
    const double hexagonRadius_ = 40.0;
    
    // Smart pointer vers la fenêtre
    std::unique_ptr<Window> window_;

    // La grille d'hexagones
    std::optional<GameMap> map_;

    // Zoom du contenu
    double zoom_ = 1.0;

    bool loop_ = true;
};

#endif
