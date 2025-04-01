#include "Game.hpp"
#include "HexagonUtils.hpp"
#include "HexagonGrid.hpp"
#include <iostream>
#include <stdexcept>

#include <memory>
#include <vector>

Game::Game() 
    : window_(std::make_unique<Window>("Konkr", windowWidth_, windowHeight_)) {
    if (!window_ || !window_->isInitialized())
        throw std::runtime_error("Échec de l'initialisation de SDL");

    renderer_ = window_->getRenderer();
    map_.emplace(renderer_, gridSize_, mapSize_, mapPos_, hexagonRadius_);
}

Game::~Game() = default;

void Game::run() {
    SDL_Event event;
    bool loop = true;
    
    while (loop) {
        // Traitement des événements via lambda
        auto processEvent = [&]() {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    loop = false;
                
                else if (event.type == SDL_MOUSEWHEEL) {
                    // Ajuste le rayon et recrée la grille si besoin
                    zoom_ *= (event.wheel.preciseY > 0) ? 1.1 : 0.9;
                    map_ = GameMap(renderer_, gridSize_, mapSize_, mapPos_, hexagonRadius_, zoom_);
                }

                map_->handleEvent(event);
            }
        };
        processEvent();

        // Rendu
        SDL_SetRenderDrawColor(renderer_, 50, 125, 160, 255);
        SDL_RenderClear(renderer_);
        map_->draw();
        SDL_RenderPresent(renderer_);
        SDL_Delay(1/60);
    }
}
