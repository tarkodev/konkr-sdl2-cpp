#include "GameMenu.hpp"
#include "SDL.h"
#include "ColorUtils.hpp"
#include "GameMap.hpp"
#include "Overlay.hpp"
#include <memory>
#include <string>

GameMenu::GameMenu(const std::shared_ptr<Window>& window, const std::string& mapPath): MenuBase{window} {
    windowSize_ = window_->getSize();

    // Init variables
    minHexSize_ = windowSize_ * 0.03;
    maxHexSize_ = windowSize_ * 0.13;

    // Create map
    createMap(mapPath);

    // Create Overlay
    overlay_ = std::make_unique<Overlay>(Point{0, 0});
    overlay_->setPos(Point{windowSize_.getWidth() / 2, windowSize_.getHeight()} - overlay_->getSize() / 2);
}

void GameMenu::createMap(const std::string& mapPath) {
    // Create map
    map_ = std::make_unique<GameMap>(Point{0, 0}, windowSize_ * 0.75, mapPath);

    // Define utils variables
    mapCellSize_ = Size{map_->getWidth(), map_->getHeight()};
    minMapSize_ = Size{
        static_cast<int>(map_->getWidth() * minHexSize_.getWidth()),
        static_cast<int>(map_->getHeight() * HexagonUtils::radiusToInner(minHexSize_.getHeight()))
    };
    maxMapSize_ = Size{
        static_cast<int>(map_->getWidth() * maxHexSize_.getWidth()),
        static_cast<int>(map_->getHeight() * HexagonUtils::radiusToInner(maxHexSize_.getHeight()))
    };

    // Recalculation of size of map
    Size mapSize = map_->getSize();
    mapSize = Size{
        std::min(std::max(mapSize.getWidth(), minMapSize_.getWidth()), maxMapSize_.getWidth()),
        std::min(std::max(mapSize.getHeight(), minMapSize_.getHeight()), maxMapSize_.getHeight())
    };
    map_->setProportionalSize(mapSize);

    // Recalculation of centering
    mapSize = map_->getSize();
    mapPos_ = Point{(windowSize_.getWidth() - mapSize.getWidth()) / 2, (windowSize_.getHeight() - mapSize.getHeight())/2};
    updateMapPos();
}

void GameMenu::updateMapPos() {
    // Get dimensions
    Size mapSize = map_->getSize();
    Point mapPos = map_->getPos();
    Point minMapPos = mapSize * 5 / mapCellSize_ - mapSize;
    Point maxMapPos = windowSize_ - mapSize * 5 / mapCellSize_;

    // Set map pos
    mapPos_.setX((mapPos_.getX() > mapPos.getX()) ? std::min(mapPos_.getX(), maxMapPos.getX()) : std::max(mapPos_.getX(), minMapPos.getX()));
    mapPos_.setY((mapPos_.getY() > mapPos.getY()) ? std::min(mapPos_.getY(), maxMapPos.getY()) : std::max(mapPos_.getY(), minMapPos.getY()));
    map_->setPos(mapPos_);
}

void GameMenu::onMouseButtonDown(SDL_Event& event) {
    map_->handleEvent(event);
    if (map_->hasTroopSelected()) return;
    
    moveOrigin_ = Point{event.motion.x, event.motion.y};
    moved_ = false;
}

void GameMenu::onMouseMotion(SDL_Event& event) {
    if (moveOrigin_) {
        // Move map
        mapPos_ += Point{event.motion.x, event.motion.y} - *moveOrigin_;
        updateMapPos();

        // Set new origin
        moveOrigin_ = {event.motion.x, event.motion.y};
        moved_ = true;
    }

    map_->handleEvent(event);
}

void GameMenu::onMouseButtonUp(SDL_Event& event) {
    map_->handleEvent(event);
    moveOrigin_.reset();
}

void GameMenu::onMouseWheel(SDL_Event& event) {
    double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;
    Size mapSize = map_->getSize() * zoom;

    Rect bounds{Point{minMapSize_}, Size{maxMapSize_ - minMapSize_}};
    if (!bounds.contains(mapSize)) return;
    
    // udpate map size
    map_->setProportionalSize(mapSize);        
    mapPos_ += ((windowSize_ / 2) - mapPos_) * (1 - zoom);
    updateMapPos();
}

void GameMenu::onKeyDown(SDL_Event& event) {
    if (event.key.keysym.sym == SDLK_RETURN) {
        map_->nextPlayer();
        return;
    }

    Point mapPos = map_->getPos();
    if (event.key.keysym.sym == SDLK_LEFT)
        mapPos_.addX(30);
    else if (event.key.keysym.sym == SDLK_RIGHT)
        mapPos_.addX(-30);
    else if (event.key.keysym.sym == SDLK_UP)
        mapPos_.addY(30);
    else if (event.key.keysym.sym == SDLK_DOWN)
        mapPos_.addY(-30);

    updateMapPos();
}

void GameMenu::handleEvents(){
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        overlay_->handleEvent(event);
        
        handleEvent(event);
        if (event.type == SDL_MOUSEBUTTONDOWN)
            onMouseButtonDown(event);
        else if (event.type == SDL_MOUSEMOTION)
            onMouseMotion(event);
        else if (event.type == SDL_MOUSEBUTTONUP)
            onMouseButtonUp(event);
        else if (event.type == SDL_MOUSEWHEEL)
            onMouseWheel(event);
        else if (event.type == SDL_KEYDOWN)
            onKeyDown(event);
    }
}

void GameMenu::draw() {
    window_->fill(ColorUtils::SEABLUE);
    
    map_->display(window_);
    overlay_->display(window_);

    window_->refresh();
}

std::shared_ptr<MenuBase> GameMenu::run() {
    loop_ = true;

    while (loop_) {
        // Handle events
        handleEvents();

        // Draw elements
        draw();

        // Control loop duration
        SDL_Delay(1/60);
    }

    return nextMenu_;
}
