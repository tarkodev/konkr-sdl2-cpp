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
#include "Font.hpp"
#include "Displayer.hpp"
#include "TreasuryDisplayer.hpp"

#include <memory>
#include <vector>


Game::Game() {
    // Create Window
    window_ = std::make_shared<Window>("Konkr", windowSize_.getWidth(), windowSize_.getHeight());
    if (!window_ || !window_->isInitialized())
        throw std::runtime_error("Échec de l'initialisation de SDL: " + std::string(SDL_GetError()));

    // Get renderer and window size
    renderer_ = window_->getRenderer();
    windowSize_ = window_->getSize();

    // Init each class to initialize
    Displayer::init(renderer_);
    Ground::init();
    Forest::init();
    PlayableGround::init();
    TreasuryDisplayer::init();

    Troop::init();
    Town::init();
    Castle::init();
    Camp::init();
    Bandit::init();
    Villager::init();
    Pikeman::init();
    Knight::init();
    Hero::init();

    Player::init(renderer_);

    GameMap::init();
    
    // Init variables
    minHexSize_ = windowSize_ * 0.03;
    maxHexSize_ = windowSize_ * 0.13;

    menu_.reset(new MainMenu(window_));

    //! A déplacer dans Overlay pour le chargement des textures
    // Après avoir créé vos textures :
    /*
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
    */
    //! A déplacer dans Overlay pour le chargement des textures
}

void Game::updateMapPos() {
    Size mapSize = map_->getSize();
    Point mapPos = map_->getPos();
    Point minMapPos = mapSize * 5 / mapCellSize_ - mapSize;
    Point maxMapPos = windowSize_ - mapSize * 5 / mapCellSize_;

    mapPos_.setX((mapPos_.getX() > mapPos.getX()) ? std::min(mapPos_.getX(), maxMapPos.getX()) : std::max(mapPos_.getX(), minMapPos.getX()));
    mapPos_.setY((mapPos_.getY() > mapPos.getY()) ? std::min(mapPos_.getY(), maxMapPos.getY()) : std::max(mapPos_.getY(), minMapPos.getY()));
    map_->setPos(mapPos_);
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
                ;//activeMenu_->handleEvent(event);

            continue;
        }

        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN: {
                map_->handleEvent(event);
                if (!(map_->hasTroopSelected())) {
                    moveOrigin_ = Point{event.motion.x, event.motion.y};
                    moved_ = false;
                }
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                moveOrigin_.reset();
                map_->handleEvent(event);
                break;
            }

            case SDL_MOUSEMOTION: {
                if (moveOrigin_.has_value()) {
                    mapPos_ += Point{event.motion.x, event.motion.y} - *moveOrigin_;
                    updateMapPos();

                    moveOrigin_ = {event.motion.x, event.motion.y};
                    moved_ = true;
                }

                map_->handleEvent(event);
                break;
            }

            case SDL_MOUSEWHEEL: {
                double zoom = (event.wheel.preciseY > 0) ? 1.1 : 0.9;
                Size mapSize = map_->getSize() * zoom;

                //! Ecrire operateur comparaison de deux Size
                if (minMapSize_.getWidth() < mapSize.getWidth() && mapSize.getWidth() < maxMapSize_.getWidth() && 
                    minMapSize_.getHeight() < mapSize.getHeight() && mapSize.getHeight() < maxMapSize_.getHeight()) {
                    map_->setProportionalSize(mapSize);
                    
                    mapPos_ += ((windowSize_ / 2) - mapPos_) * (1 - zoom);
                    updateMapPos();
                }
                break;
            }

            case SDL_KEYDOWN: {
                Point mapPos = map_->getPos();

                if (event.key.keysym.sym == SDLK_RETURN)
                    map_->nextPlayer();
                else {
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
    window_->fill({60, 104, 131});

    if (screen_ == ScreenState::InGame) {
        map_->display(&(*window_));

        // Dans la boucle de rendu :
        overlay_.display(&(*window_));
    } else if (activeMenu_) {
        //activeMenu_->display(&(*window_));
    }
    
    window_->refresh();
}

void Game::openMapSelect() {
    screen_ = ScreenState::MapSelect;
    activeMenu_ = std::make_unique<MapSelectMenu>(window_);
}

void Game::openMainMenu() {
    screen_     = ScreenState::MainMenu;
    activeMenu_ = std::make_unique<MainMenu>(window_);
}

void Game::startGameWithMap(const std::string& file) {
    map_.reset();
    map_.emplace(Point{0, 0}, windowSize_*0.75, file);
    screen_     = ScreenState::InGame;
    activeMenu_.reset();

    mapCellSize_ = Size{map_->getWidth(), map_->getHeight()};

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
    mapPos_ = Point{(windowSize_.getWidth() - mapSize.getWidth()) / 2, (windowSize_.getHeight() - mapSize.getHeight())/2};
    updateMapPos();
}

void Game::run() {
    while (menu_)
        menu_ = menu_->run();
}
