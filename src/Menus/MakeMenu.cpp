#include "Menus/MakeMenu.hpp"
#include "SDL.h"
#include "Utils/ColorUtils.hpp"
#include "Widgets/GameMap.hpp"
#include "Widgets/Overlay.hpp"
#include <memory>
#include <string>
#include "Utils/Checker.hpp"
#include "SDLWrappers/Cursor.hpp"
#include "Menus/MainMenu.hpp"
#include "Utils/HexagonUtils.hpp"
#include "Cells/Grounds/PlayableGround.hpp"
#include "Cells/Water.hpp"
#include "Cells/Grounds/Forest.hpp"
#include "GameElements/Town.hpp"
#include "GameElements/Castle.hpp"
#include "GameElements/Camp.hpp"
#include "GameElements/Troops/Bandit.hpp"
#include "GameElements/Troops/Hero.hpp"
#include "GameElements/Troops/Knight.hpp"
#include "GameElements/Troops/Pikeman.hpp"
#include "GameElements/Troops/Troop.hpp"
#include "GameElements/Troops/Villager.hpp"


MakeMenu::MakeMenu(const std::shared_ptr<Window>& window): MenuBase{window} {
    windowSize_ = window_->getSize();

    // Init variables
    minHexSize_ = windowSize_ * 0.03;
    maxHexSize_ = windowSize_ * 0.13;

    // Create map
    createMap("../assets/map/Base/Base.ascii");

    // Create players
    for (int i = 1; i < 10; i++) {
        auto p = std::make_shared<Player>(ColorUtils::getGroundColor(i));
        p->setNum(i);
        players_.push_back(p);
    }

    // Create Back button
    backBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/back_btn.png", "../assets/img/buttons/back_btn_hover.png", "../assets/img/buttons/back_btn_pressed.png");
    backBtn_->setPos(Point{backBtn_->getWidth() / 2, window_->getHeight() - backBtn_->getHeight() / 2});
    backBtn_->setCallback([this]() { nextMenu_ = std::make_shared<MainMenu>(window_); loop_ = false; });
}

void MakeMenu::createMap(const std::string& mapPath) {
    // Create map
    map_ = std::make_unique<GameMap>(Point{0, 0}, windowSize_ * 0.75, mapPath);
    updateMapPosAndSize();
}

void MakeMenu::updateMapPosAndSize() {
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
    Rect bounds{Point{minMapSize_}, Size{maxMapSize_ - minMapSize_}};
    if (!bounds.contains(mapSize))
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

void MakeMenu::updateMapPos() {
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

void MakeMenu::onMouseButtonDown(SDL_Event& event) {
    Point mp{event.motion.x, event.motion.y};
    if (backBtn_->isHover(mp))
        return;
    
    moveOrigin_ = mp;
    moved_ = false;
}

void MakeMenu::onMouseMotion(SDL_Event& event) {
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

void MakeMenu::onMouseButtonUp(SDL_Event& event) {
    if (!moved_) {
        if (auto cell = map_->getSelectedCell().lock()) {
            if (Water::is(cell))
                map_->setInSelectedCell(std::make_shared<PlayableGround>(Point{0, 0}));
            else
                map_->setInSelectedCell(std::make_shared<Water>());
        }
    }
    
    moveOrigin_.reset();
}

void MakeMenu::onMouseWheel(SDL_Event& event) {
    double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;
    Size mapSize = map_->getSize() * zoom;

    Rect bounds{Point{minMapSize_}, Size{maxMapSize_ - minMapSize_}};
    if (!bounds.contains(mapSize)) return;
    
    // udpate map size
    map_->setProportionalSize(mapSize);        
    mapPos_ += ((windowSize_ / 2) - mapPos_) * (1 - zoom);
    updateMapPos();
}

void MakeMenu::onKeyDown(SDL_Event& event) {
    if (event.key.keysym.sym == SDLK_RETURN) {
        auto cell = map_->getSelectedCell().lock();
        if (!cell) return;

        auto ground = Ground::cast(cell);
        if (!ground) return;
        
        if (Forest::is(ground))
            map_->setInSelectedCell(std::make_shared<PlayableGround>(Point{0, 0}, players_[0]));
        else {
            auto pg = PlayableGround::cast(ground);
            if (!pg->getOwner()) map_->setInSelectedCell(std::make_shared<Forest>(Point{0, 0}));
            else pg->setOwner(players_[pg->getOwner()->getNum() % 9]);
        }
        return;
    }

    else if (event.key.keysym.sym == SDLK_SPACE) {
        auto cell = map_->getSelectedCell().lock();
        if (!cell) return;

        auto pg = PlayableGround::cast(cell);
        if (!pg) return;

        if (!pg->getOwner()) {
            if (Camp::is(pg->getElement())) pg->setElement(std::make_shared<Bandit>(Point{0, 0}));
            else pg->setElement(std::make_shared<Camp>(Point{0, 0}));
            return;
        }

        auto elt = pg->getElement();
        if (!elt) pg->setElement(std::make_shared<Town>(Point{0, 0}));
        else if (Bandit::is(elt)) pg->setElement(std::make_shared<Town>(Point{0, 0}));
        else if (Hero::is(elt)) pg->setElement(std::make_shared<Bandit>(Point{0, 0}));
        else if (Knight::is(elt)) pg->setElement(std::make_shared<Hero>(Point{0, 0}));
        else if (Pikeman::is(elt)) pg->setElement(std::make_shared<Knight>(Point{0, 0}));
        else if (Villager::is(elt)) pg->setElement(std::make_shared<Pikeman>(Point{0, 0}));
        else if (Castle::is(elt)) pg->setElement(std::make_shared<Villager>(Point{0, 0}));
        else if (Town::is(elt)) pg->setElement(std::make_shared<Castle>(Point{0, 0}));

        return;
    }

    else if (event.key.keysym.sym == SDLK_s) {
        map_->saveMap();
    }

    Point mapPos = map_->getPos();
    if (event.key.keysym.sym == SDLK_LEFT) {
        map_->addWidth(-1);
        map_->setProportionalSize(windowSize_ * 0.75);
        updateMapPosAndSize();
    } else if (event.key.keysym.sym == SDLK_RIGHT) {
        map_->addWidth(1);
        map_->setProportionalSize(windowSize_ * 0.75);
        updateMapPosAndSize();
    } else if (event.key.keysym.sym == SDLK_UP) {
        map_->addHeight(-1);
        map_->setProportionalSize(windowSize_ * 0.75);
        updateMapPosAndSize();
    } else if (event.key.keysym.sym == SDLK_DOWN) {
        map_->addHeight(1);
        map_->setProportionalSize(windowSize_ * 0.75);
        updateMapPosAndSize();
    }

    updateMapPos();
}

void MakeMenu::handleEvents(){
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        handleEvent(event);
        backBtn_->handleEvent(event);
        
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

void MakeMenu::draw() {
    window_->fill(ColorUtils::SEABLUE);
    
    map_->display(window_);
    backBtn_->display(window_);

    window_->refresh();
    frameCount_++;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastLogTime_ >= 3000) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "FPS: %.2f", static_cast<float>(frameCount_) / ((currentTime - lastLogTime_) / 1000.0f));
        lastLogTime_ = currentTime;
        frameCount_ = 0;
    }
}

std::shared_ptr<MenuBase> MakeMenu::run() {
    lastLogTime_ = SDL_GetTicks();
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
