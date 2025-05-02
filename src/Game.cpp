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

    // Select the current menu
    menu_.reset(new MainMenu(window_));
}

Game::~Game() = default;

void Game::run() {
    while (menu_)
        menu_ = menu_->run();
}
