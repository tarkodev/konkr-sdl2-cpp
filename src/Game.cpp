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
    window_ = std::make_shared<Window>("Konkr", windowSize_);

    // Get renderer and window size
    std::shared_ptr<SDL_Renderer> renderer = window_->getRenderer();

    // Init each class to initialize
    Displayer::init(renderer);
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

    Player::init(renderer);
    GameMap::init();

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

Game::~Game() = default;

void Game::run() {
    while (menu_)
        menu_ = menu_->run();
}
