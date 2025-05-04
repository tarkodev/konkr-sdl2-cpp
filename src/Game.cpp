#include "Game.hpp"
#include "HexagonUtils.hpp"
#include "HexagonGrid.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Cells/Water.hpp"
#include "Cells/Grounds/Ground.hpp"
#include "Cells/Grounds/PlayableGround.hpp"
#include "Cells/Grounds/PlayableGround.hpp"
#include "Cells/Grounds/Forest.hpp"
#include <iostream>
#include <stdexcept>
#include "GameElements/Troops/Bandit.hpp"
#include "GameElements/Town.hpp"
#include "GameElements/Castle.hpp"
#include "GameElements/Camp.hpp"
#include "GameElements/Troops/Villager.hpp"
#include "GameElements/Troops/Pikeman.hpp"
#include "GameElements/Troops/Knight.hpp"
#include "GameElements/Troops/Hero.hpp"
#include "Button.hpp"
#include "SDLWrappers/Font.hpp"
#include "Displayer.hpp"
#include "TreasuryDisplayer.hpp"
#include "SDLWrappers/Cursor.hpp"

#include <memory>
#include <vector>


Game::Game() {
    // Create Window
    window_ = std::make_shared<Window>("Konkr", windowSize_);

    // Get renderer and window size
    std::shared_ptr<SDL_Renderer> renderer = window_->getRenderer();

    // Init each class to initialize
    Cursor::init();
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

Game::~Game() {
    Player::quit();

    Hero::quit();
    Knight::quit();
    Pikeman::quit();
    Villager::quit();
    Bandit::quit();
    Camp::quit();
    Castle::quit();
    Town::quit();
    Troop::quit();
    
    TreasuryDisplayer::quit();
    PlayableGround::quit();
    Forest::quit();
    Ground::quit();
    Cursor::quit();
}

void Game::run() {
    while (menu_)
        menu_ = menu_->run();
}
