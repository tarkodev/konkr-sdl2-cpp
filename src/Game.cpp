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
    //mapPos_ = {(windowSize_.getWidth() - mapRealSize.getWidth()) / 2, (windowSize_.getHeight() - mapRealSize.getHeight()) / 2};




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
            if (activeMenu_) activeMenu_->handleEvent(event);
            continue;                    // ne propage pas l’événement au jeu
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
            if (moveOrigin_.has_value()) {
                moved_ = true;
                SDL_Point origin = moveOrigin_.value();
                mapPos_ += Point{(event.motion.x - origin.x), (event.motion.y - origin.y)};
                moveOrigin_ = {event.motion.x, event.motion.y};

            } else {
                //! mettre les pos dans les objets et ne plus passer de pos à display
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
            else if (event.key.keysym.sym == SDLK_RETURN)
                map_->endTurn();
            break;
        }
        }

        //map_->handleEvent(event);
        overlay_.handleEvent(event);
    }

    switch (pendingAction_) {
        case PendingAction::OpenMapSelect: openMapSelect(); break;
        case PendingAction::OpenMainMenu:  openMainMenu();  break;
        default: break;
    }
    pendingAction_ = PendingAction::None;
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer_, bgColor.r,bgColor.g,bgColor.b,bgColor.a);
    SDL_RenderClear(renderer_);

    if (screen_ == ScreenState::InGame) {
        map_->draw(mapPos_);

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
    map_.reset();                     // vide l’ancienne éventuelle
    map_.emplace(windowSize_*0.75, file);
    screen_     = ScreenState::InGame;
    activeMenu_.reset();
    // recalcul du centrage :
    Size mapSize = map_->getSize();
    mapPos_ = {(windowSize_.getWidth()-mapSize.getWidth())/2,
               (windowSize_.getHeight()-mapSize.getHeight())/2};
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
