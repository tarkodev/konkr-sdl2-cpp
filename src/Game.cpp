#include "Game.hpp"
#include "HexagonUtils.hpp"
#include "HexagonGrid.hpp"
#include "Texture.hpp"
#include "Water.hpp"
#include "Ground.hpp"
#include "PlayableGround.hpp"
#include "PlayableGround.hpp"
#include "Forest.hpp"
#include <iostream>
#include <stdexcept>
#include "logic/units/Bandit.hpp"
#include "logic/units/Town.hpp"
#include "logic/units/Castle.hpp"
#include "logic/units/Camp.hpp"
#include "logic/units/Villager.hpp"
#include "logic/units/Pikeman.hpp"
#include "logic/units/Knight.hpp"
#include "logic/units/Hero.hpp"
#include "Button.hpp"

#include <memory>
#include <vector>


Game::Game() 
    : window_(std::make_unique<Window>("Konkr", windowSize_.getWidth(), windowSize_.getHeight())) {
    if (!window_ || !window_->isInitialized())
        throw std::runtime_error("Échec de l'initialisation de SDL: " + std::string(SDL_GetError()));

    pendingAction_ = PendingAction::None;


    // Get renderer and window size
    renderer_ = window_->getRenderer();
    windowSize_ = window_->getSize();

    minHexSize_ = windowSize_ * 0.03;
    maxHexSize_ = windowSize_ * 0.13;

    // Init each class to initialize
    Cell::init(renderer_);
    Ground::init();
    Forest::init();
    PlayableGround::init();

    GameElement::init(renderer_);
    Troop::init();
    Bandit::init();
    Town::init();
    Castle::init();
    Camp::init();
    Villager::init();
    Pikeman::init();
    Knight::init();
    Hero::init();

    Player::init(renderer_);

    GameMap::init(renderer_);
    

    openMainMenu();


    // Create map;
    //map_.emplace(windowSize_ * 0.75, "../assets/map/Unity.txt");


    //map_.emplace(windowSize_ * 0.75, "../assets/map/Unity.txt");

    //Size mapRealSize = map_->getSize();
    //map->setPos({(windowSize_.getWidth() - mapRealSize.getWidth()) / 2, (windowSize_.getHeight() - mapRealSize.getHeight()) / 2});




    //! A déplacer dans Overlay pour le chargement des textures
    // Après avoir créé vos textures :
    Texture* undoTex  = new Texture(renderer_, "../assets/img/undo.png");
    Button* undoBtn = new Button(undoTex, nullptr, nullptr, Point{150,100});
    undoBtn->setCallback([](){
        std::cout << "Undo button clicked!" << std::endl;
    });

    Texture* turnTex  = new Texture(renderer_, "../assets/img/turn.png");
    Button* turnBtn = new Button(turnTex, nullptr, nullptr, Point{150,200});
    turnBtn->setCallback([](){
        std::cout << "Turn button clicked!" << std::endl;
    });

    Texture* nextTex  = new Texture(renderer_, "../assets/img/next.png");
    Button* nextBtn = new Button(nextTex, nullptr, nullptr, Point{150,300});
    nextBtn->setCallback([](){
        std::cout << "Next button clicked!" << std::endl;
    });

    Texture* skipTex  = new Texture(renderer_, "../assets/img/skip.png");
    Button* skipBtn = new Button(skipTex, nullptr, nullptr, Point{150,400});
    skipBtn->setCallback([](){
        std::cout << "Skip button clicked!" << std::endl;
    });

    Texture* backTex = new Texture(renderer_, "../assets/img/back.png");
    Button* backBtn = new Button(backTex, nullptr, nullptr, Point{150, 500});
    backBtn->setCallback([this]() {
        this->openMapSelect();
    });
    overlay_.addButton(backBtn);


    overlay_.addButton(undoBtn);
    overlay_.addButton(turnBtn);
    overlay_.addButton(nextBtn);
    overlay_.addButton(skipBtn);
    //! A déplacer dans Overlay pour le chargement des textures
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
        }

        if (screen_ != ScreenState::InGame) {
            if (activeMenu_)
                activeMenu_->handleEvent(event);

            continue;
        }

        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN: {
                map_->handleEvent(event);
                if (!(map_->hasTroopSelected())) {
                    moveOrigin_ = {event.motion.x, event.motion.y};
                    moved_ = false;
                }
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                moveOrigin_.reset();
                if (!moved_)
                    map_->test();
                map_->handleEvent(event);
                break;
            }

            case SDL_MOUSEMOTION: {
                Point mapPos = map_->getPos();

                if (moveOrigin_.has_value()) {
                    moved_ = true;
                    SDL_Point origin = moveOrigin_.value();

                    mapPos += Point{(event.motion.x - origin.x), (event.motion.y - origin.y)};
                    map_->setPos(mapPos);

                    moveOrigin_ = {event.motion.x, event.motion.y};
                }
                map_->handleEvent(event);
                break;
            }

            case SDL_MOUSEWHEEL: {
                double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;
                Size mapSize = map_->getSize() * zoom;

                if (minMapSize_.getWidth() < mapSize.getWidth() && mapSize.getWidth() < maxMapSize_.getWidth() && 
                    minMapSize_.getHeight() < mapSize.getHeight() && mapSize.getHeight() < maxMapSize_.getHeight()) {
                    map_->setProportionalSize(mapSize);
                    Point mapPos = map_->getPos();
                    map_->setPos(mapPos + ((windowSize_ / 2) - mapPos) * (1 - zoom));
                }
                break;
            }

            case SDL_KEYDOWN: {
                Point mapPos = map_->getPos();

                if (event.key.keysym.sym == SDLK_LEFT)
                    mapPos.addX(10);
                else if (event.key.keysym.sym == SDLK_RIGHT)
                    mapPos.addX(-10);
                else if (event.key.keysym.sym == SDLK_UP)
                    mapPos.addY(10);
                else if (event.key.keysym.sym == SDLK_DOWN)
                    mapPos.addY(-10);
                else if (event.key.keysym.sym == SDLK_RETURN)
                    map_->endTurn();

                map_->setPos(mapPos);
                break;
            }
        }

        //map_->handleEvent(event);
        overlay_.handleEvent(event);
    }

    switch (pendingAction_) {
        case PendingAction::OpenMapSelect: openMapSelect(); break;
        case PendingAction::OpenMainMenu:  openMainMenu();  break;
        case PendingAction::StartGameWithMap:
            startGameWithMap(pendingMapFile_);
            break;
        default: break;
    }
    pendingAction_ = PendingAction::None;
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer_, bgColor.r,bgColor.g,bgColor.b,bgColor.a);
    SDL_RenderClear(renderer_);

    if (screen_ == ScreenState::InGame) {
        map_->display(nullptr);

        // Dans la boucle de rendu :
        overlay_.render(renderer_);
    } else if (activeMenu_) {
        activeMenu_->render(renderer_);
    }

    SDL_RenderPresent(renderer_);  
}

void Game::openMapSelect() {
    screen_ = ScreenState::MapSelect;
    activeMenu_ = std::make_unique<MapSelectMenu>(renderer_, *this);
}

void Game::openMainMenu() {
    screen_     = ScreenState::MainMenu;
    activeMenu_ = std::make_unique<MainMenu>(renderer_, *this);
}

void Game::startGameWithMap(const std::string& file) {
    map_.reset();
    map_.emplace(Point{0, 0}, windowSize_*0.75, file);
    screen_     = ScreenState::InGame;
    activeMenu_.reset();

    minMapSize_ = Size{
        static_cast<int>(map_->getWidth() * minHexSize_.getWidth()),
        static_cast<int>(map_->getHeight() * HexagonUtils::radiusToInner(minHexSize_.getHeight()))
    };

    maxMapSize_ = Size{
        static_cast<int>(map_->getWidth() * maxHexSize_.getWidth()),
        static_cast<int>(map_->getHeight() * HexagonUtils::radiusToInner(maxHexSize_.getHeight()))
    };

   // recalcul de la taille de la map
    Size mapSize = map_->getSize();
    mapSize = Size{
        std::min(std::max(mapSize.getWidth(), minMapSize_.getWidth()), maxMapSize_.getWidth()),
        std::min(std::max(mapSize.getHeight(), minMapSize_.getHeight()), maxMapSize_.getHeight())
    };
    map_->setProportionalSize(mapSize);

    // recalcul du centrage :
    mapSize = map_->getSize();
    map_->setPos({(windowSize_.getWidth() - mapSize.getWidth()) / 2, (windowSize_.getHeight() - mapSize.getHeight())/2});
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
