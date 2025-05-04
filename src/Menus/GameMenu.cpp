#include "Menus/GameMenu.hpp"
#include "SDL.h"
#include "Utils/ColorUtils.hpp"
#include "Widgets/GameMap.hpp"
#include "Widgets/Overlay.hpp"
#include <memory>
#include <string>
#include "Utils/Checker.hpp"
#include "SDLWrappers/Cursor.hpp"
#include "Menus/MapsMenu.hpp"
#include "Utils/HexagonUtils.hpp"

GameMenu::GameMenu(const std::shared_ptr<Window>& window, const std::string& mapPath): MenuBase{window} {
    windowSize_ = window_->getSize();

    // Init variables
    minHexSize_ = windowSize_ * 0.03;
    maxHexSize_ = windowSize_ * 0.13;

    // Create map
    createMap(mapPath);

    // Create Overlay
    overlay_ = std::make_unique<Overlay>(Point{0, 0});
    overlay_->setPos(Point{windowSize_.getWidth() / 2, windowSize_.getHeight() - overlay_->getHeight() / 2});

    // Create win texture
    finishTex_ = std::make_shared<Texture>(window_->getRenderer(), "../assets/img/win.png");

    // Create finish game button
    finishBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/finish_btn.png", "../assets/img/buttons/finish_btn_hover.png", "../assets/img/buttons/finish_btn_pressed.png");
    finishBtn_->setPos(windowSize_ / 2 + Point{0, (finishTex_->getHeight() + finishBtn_->getHeight()) / 2});
    finishBtn_->setCallback([this]() {
        nextMenu_ = std::make_shared<MapsMenu>(window_);
        loop_ = false; 
    });
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
    if (Rect{Point{0, 0}, minMapSize_}.contains(mapSize))
        minMapSize_ = mapSize;
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

void GameMenu::updateShop() {
    overlay_->update(map_->getMaxTreasuryOfCurrentPlayer());
}

const bool GameMenu::handleOverlay(SDL_Event& event) {
    overlay_->handleEvent(event);

    // If return to MapsMenu
    if (overlay_->backRequested()) {
        nextMenu_ = std::make_shared<MapsMenu>(window_);
        loop_ = false;
        return true;
    }

    // If next turn
    else if (overlay_->turnRequested()) {
        map_->nextPlayer();
        return true;
    }

    // If Troop bought
    else if (overlay_->buyTroopRequested()) {
        map_->buyTroop(overlay_->getTroopBought());
        return true;
    }

    return false;
}

void GameMenu::onMouseButtonDown(SDL_Event& event) {
    Point mp{event.motion.x, event.motion.y};
    if (overlay_->isHover(mp)) return;

    map_->handleEvent(event);
    if (map_->hasTroopSelected()) return;
    
    moveOrigin_ = mp;
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

    if (true || map_->hasTroopSelected())
        map_->handleEvent(event);
}

void GameMenu::onMouseButtonUp(SDL_Event& event) {
    map_->handleEvent(event);
    moveOrigin_.reset();
    updateShop();
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
        updateShop();
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
        handleEvent(event);

        // If game finished
        if (gameFinished_) {
            finishBtn_->handleEvent(event);
            continue;
        }

        // Handle event of overlay
        if (handleOverlay(event)) {
            updateShop();
            continue;
        }
        
        // handle event of menu
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
    Cursor::update();
}

void GameMenu::draw() {
    window_->fill(ColorUtils::SEABLUE);
    
    map_->display(window_);
    overlay_->display(window_);

    if (gameFinished_) {
        window_->darken();
        window_->blit(finishTex_, (windowSize_ - finishTex_->getSize()) / 2);
        finishBtn_->display(window_);
    }

    window_->refresh();
    frameCount_++;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastLogTime_ >= 3000) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "FPS: %.2f", static_cast<float>(frameCount_) / ((currentTime - lastLogTime_) / 1000.0f));
        lastLogTime_ = currentTime;
        frameCount_ = 0;
    }
}

std::shared_ptr<MenuBase> GameMenu::run() {
    lastLogTime_ = SDL_GetTicks();
    updateShop();
    loop_ = true;

    while (loop_) {
        // Handle events
        handleEvents();
        gameFinished_ = map_->gameFinished();

        // Draw elements
        draw();

        // Control loop duration
        SDL_Delay(1/60);
    }

    return nextMenu_;
}
