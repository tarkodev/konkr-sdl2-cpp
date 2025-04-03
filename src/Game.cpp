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
    map_.emplace(renderer_, mapPos_, mapSize_, gridSize_);
}

Game::~Game() = default;

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT: {
            loop_ = false;
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            moveOrigin_ = {event.motion.x, event.motion.y};
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            moveOrigin_.reset();
            break;
        }

        case SDL_MOUSEMOTION: {
            if (!moveOrigin_.has_value()) break;
            SDL_Point origin = moveOrigin_.value();
            map_->addX(event.motion.x - origin.x);
            map_->addY(event.motion.y - origin.y);
            moveOrigin_ = {event.motion.x, event.motion.y};
            break;
        }

        case SDL_MOUSEWHEEL: {
            SDL_Point mapPos = map_->getPos();
            SDL_Rect mapSize = map_->getSize();
            double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;

            mapSize.w *= zoom;
            mapSize.h *= zoom;
            map_->setProportionalSize(mapSize);

            mapPos.x -= (mapPos.x - windowWidth_ / 2.0) * (1 - zoom);
            mapPos.y -= (mapPos.y - windowHeight_ / 2.0) * (1 - zoom);
            map_->setPos(mapPos);
            break;
        }

        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_LEFT)
                map_->addX(10);
            else if (event.key.keysym.sym == SDLK_RIGHT)
                map_->addX(-10);
            else if (event.key.keysym.sym == SDLK_UP)
                map_->addY(10);
            else if (event.key.keysym.sym == SDLK_DOWN)
                map_->addY(-10);
            break;
        }
        }

        map_->handleEvent(event);
    }
}

void Game::draw() {
    // Fill background
    SDL_SetRenderDrawColor(renderer_, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
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
