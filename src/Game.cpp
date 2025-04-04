#include "Game.hpp"
#include "HexagonUtils.hpp"
#include "HexagonGrid.hpp"
#include "DrawUtils.hpp"
#include <iostream>
#include <stdexcept>

#include <memory>
#include <vector>

Game::Game() 
    : window_(std::make_unique<Window>("Konkr", windowSize_.w, windowSize_.h)) {
    if (!window_ || !window_->isInitialized())
        throw std::runtime_error("Échec de l'initialisation de SDL: " + std::string(SDL_GetError()));

    // Get renderer and window size
    renderer_ = window_->getRenderer();
    windowSize_ = window_->getSize();

    // Load hexagon sprite
    SDL_Texture *hexagonSprite = IMG_LoadTexture(renderer_, "../assets/img/hexagon.png");
    if (!hexagonSprite) throw std::runtime_error("Échec du chargement de la texture des hexagones: " + std::string(SDL_GetError()));

    // Get Size
    SDL_Rect hexagonSpriteSize = DrawUtils::getSize(hexagonSprite);

    // Set default texture
    Player::setDefaultHexagonSprite(hexagonSprite, hexagonSpriteSize);

    SDL_Texture* brownHexagonSprite = DrawUtils::copyTexture(renderer_, hexagonSprite);
    
    SDL_Texture* currentTarget = SDL_GetRenderTarget(renderer_);
    SDL_SetRenderTarget(renderer_, brownHexagonSprite);
    SDL_SetTextureColorMod(brownHexagonSprite, ColorUtils::BROWN.r, ColorUtils::BROWN.g, ColorUtils::BROWN.b);
    SDL_SetRenderTarget(renderer_, currentTarget);

    Cell::setDefaultSprite(brownHexagonSprite, hexagonSpriteSize);

    // Create map
    SDL_Rect mapSize = {
        0, 0, 
        static_cast<int>(windowSize_.w * 0.25), 
        static_cast<int>(windowSize_.h * 0.25)
    };
    map_.emplace(renderer_, mapPos_, mapSize, gridSize_, hexagonSpriteSize);

    SDL_Rect mapRealSize = map_->getSize();
    mapPos_ = {(windowSize_.w - mapRealSize.w) / 2, (windowSize_.h - mapRealSize.h) / 2};

    // Create Players
    Player p1(renderer_, ColorUtils::RED);
    Player p2(renderer_, ColorUtils::BLUE);

    map_->set(2, 2, new Cell(renderer_, &p1));
    map_->refresh();
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
            if (moveOrigin_.has_value()) {
                SDL_Point origin = moveOrigin_.value();
                mapPos_.x += (event.motion.x - origin.x);
                mapPos_.y += (event.motion.y - origin.y);
                moveOrigin_ = {event.motion.x, event.motion.y};
            } else {
                map_->selectHexagon({event.motion.x - mapPos_.x, event.motion.y - mapPos_.y});
            }
            break;
        }

        case SDL_MOUSEWHEEL: {
            SDL_Rect mapSize = map_->getSize();
            double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;

            mapSize.w *= zoom;
            mapSize.h *= zoom;
            map_->setProportionalSize(mapSize);

            mapPos_.x -= (mapPos_.x - windowSize_.w / 2.0) * (1 - zoom);
            mapPos_.y -= (mapPos_.y - windowSize_.h / 2.0) * (1 - zoom);
            map_->selectHexagon({event.wheel.mouseX - mapPos_.x, event.wheel.mouseY - mapPos_.y});
            break;
        }

        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_LEFT)
                mapPos_.x += 10;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                mapPos_.x -= 10;
            else if (event.key.keysym.sym == SDLK_UP)
                mapPos_.y += 10;
            else if (event.key.keysym.sym == SDLK_DOWN)
                mapPos_.y -= 10;
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
