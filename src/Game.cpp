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

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            loop_ = false;
            break;
        }

        map_->handleEvent(event);
    }
}

void Game::draw() {
    // Fill background
    SDL_SetRenderDrawColor(renderer_, 50, 125, 160, 255);
    SDL_RenderClear(renderer_);

    map_->draw();
    
    // Show rendering
    SDL_RenderPresent(renderer_);
}

void Game::run() {
    loop_ = true;
    while (loop_) {
        // Handle events
        handleEvents();

        // Draw elements
        draw();

        // Control loop duration
        SDL_Delay(1/60);
    }
}
