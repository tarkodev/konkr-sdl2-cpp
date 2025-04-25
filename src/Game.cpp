#include "Game.hpp"
#include "HexagonUtils.hpp"
#include "HexagonGrid.hpp"
#include "Texture.hpp"
#include "Sea.hpp"
#include "Territory.hpp"
#include "PlayerTerritory.hpp"
#include "Plain.hpp"
#include "Forest.hpp"
#include <iostream>
#include <stdexcept>

#include <memory>
#include <vector>

Game::Game() 
    : window_(std::make_unique<Window>("Konkr", windowSize_.getWidth(), windowSize_.getHeight())) {
    if (!window_ || !window_->isInitialized())
        throw std::runtime_error("Échec de l'initialisation de SDL: " + std::string(SDL_GetError()));

    // Get renderer and window size
    renderer_ = window_->getRenderer();
    windowSize_ = window_->getSize();

    // Set texture in Cell
    Cell::init(renderer_);
    Territory::init();
    Plain::init();
    Forest::init();

    GameMap::init(renderer_);

    // Create map
    map_.emplace(windowSize_ * 0.75, gridSize_);
    //map_.emplace(windowSize_ * 0.75, "../assets/map/map1");

    Size mapRealSize = map_->getSize();
    mapPos_ = {(windowSize_.getWidth() - mapRealSize.getWidth()) / 2, (windowSize_.getHeight() - mapRealSize.getHeight()) / 2};

    //map_->set(0, 19, new Forest());


    /*
    map_->set(0, 0, new PlayerTerritory(&p1_));
    map_->set(0, 1, new PlayerTerritory(&p1_));
    map_->set(0, 2, new PlayerTerritory(&p1_));
    map_->set(0, 3, new PlayerTerritory(&p1_));
    map_->refresh();
    */
    
    /*
    map_->set(1, 2, new Sea());
    map_->set(3, 3, new Sea());

    map_->set(12, 8, new PlayerTerritory(&p1_));
    map_->set(12, 9, new PlayerTerritory(&p1_));
    map_->set(11, 8, new PlayerTerritory(&p1_));
    map_->set(11, 9, new PlayerTerritory(&p1_));
    map_->set(13, 8, new PlayerTerritory(&p1_));

    map_->set(3, 14, new PlayerTerritory(&p2_));
    map_->set(3, 15, new PlayerTerritory(&p2_));
    map_->set(2, 13, new PlayerTerritory(&p2_));
    map_->set(3, 12, new PlayerTerritory(&p2_));
    map_->set(3, 11, new PlayerTerritory(&p2_));

    map_->refresh();
    */

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
            moved_ = false;
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            moveOrigin_.reset();
            if (!moved_)
                map_->test();
            break;
        }

        case SDL_MOUSEMOTION: {
            if (moveOrigin_.has_value()) {
                moved_ = true;
                SDL_Point origin = moveOrigin_.value();
                mapPos_ += Point{(event.motion.x - origin.x), (event.motion.y - origin.y)};
                moveOrigin_ = {event.motion.x, event.motion.y};

            } else {
                map_->selectHexagon({event.motion.x - mapPos_.getX(), event.motion.y - mapPos_.getY()});
            }
            break;
        }

        case SDL_MOUSEWHEEL: {
            Size mapSize = map_->getSize();
            double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;

            mapSize *= zoom;
            map_->setProportionalSize(mapSize);

            mapPos_ -= Point{
                static_cast<int>((mapPos_.getX() - windowSize_.getWidth() / 2.0) * (1 - zoom)),
                static_cast<int>((mapPos_.getY() - windowSize_.getHeight() / 2.0) * (1 - zoom))
            };

            map_->selectHexagon({
                event.wheel.mouseX - mapPos_.getX(), 
                event.wheel.mouseY - mapPos_.getY()
            });
            break;
        }

        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_LEFT)
                mapPos_.addX(10);
            else if (event.key.keysym.sym == SDLK_RIGHT)
                mapPos_.addX(-10);
            else if (event.key.keysym.sym == SDLK_UP)
                mapPos_.addY(10);
            else if (event.key.keysym.sym == SDLK_DOWN)
                mapPos_.addY(-10);
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

    map_->draw(mapPos_);
    
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
