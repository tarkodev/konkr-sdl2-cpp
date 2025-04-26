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

    // Get renderer and window size
    renderer_ = window_->getRenderer();
    windowSize_ = window_->getSize();

    // Set texture in Cell
    Cell::init(renderer_);
    Ground::init();
    Forest::init();

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

    p3_.emplace("Théo", ColorUtils::BLUE);
    p3_.emplace("Kevin", ColorUtils::LIGHT_GOLDENROD_YELLOW);
    p3_.emplace("Jean", ColorUtils::RED);

    p3_.emplace("Théo", ColorUtils::BLUE);
    p3_.emplace("Kevin", ColorUtils::LIGHT_GOLDENROD_YELLOW);
    p3_.emplace("Jean", ColorUtils::RED);

    // Create map;
    map_.emplace(windowSize_ * 0.75, "../assets/map/map1");
    SDL_Log("B");

    Size mapRealSize = map_->getSize();
    mapPos_ = {(windowSize_.getWidth() - mapRealSize.getWidth()) / 2, (windowSize_.getHeight() - mapRealSize.getHeight()) / 2};

    //////////// Button

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

    overlay_.addButton(undoBtn);
    overlay_.addButton(turnBtn);
    overlay_.addButton(nextBtn);
    overlay_.addButton(skipBtn);
    //map_->set(0, 19, new Forest());


    /*
    map_->set(0, 0, new PlayableGround(&p1_));
    map_->set(0, 1, new PlayableGround(&p1_));
    map_->set(0, 2, new PlayableGround(&p1_));
    map_->set(0, 3, new PlayableGround(&p1_));
    map_->refresh();
    */
    
    /*
    map_->set(1, 2, new Water());
    map_->set(3, 3, new Water());

    map_->set(12, 8, new PlayableGround(&p1_));
    map_->set(12, 9, new PlayableGround(&p1_));
    map_->set(11, 8, new PlayableGround(&p1_));
    map_->set(11, 9, new PlayableGround(&p1_));
    map_->set(13, 8, new PlayableGround(&p1_));

    map_->set(3, 14, new PlayableGround(&p2_));
    map_->set(3, 15, new PlayableGround(&p2_));
    map_->set(2, 13, new PlayableGround(&p2_));
    map_->set(3, 12, new PlayableGround(&p2_));
    map_->set(3, 11, new PlayableGround(&p2_));

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
        overlay_.handleEvent(event);

    }
}

void Game::draw() {
    // Fill background
    SDL_SetRenderDrawColor(renderer_, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer_);

    map_->draw(mapPos_);


    // Dans la boucle de rendu :
    overlay_.render(renderer_);
    
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
