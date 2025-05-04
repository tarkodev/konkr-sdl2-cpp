#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "SDL.h"
#include "ColorUtils.hpp"
#include "GameMap.hpp"
#include "SDLWrappers/Renderers/Window.hpp"
#include "Player.hpp"
#include "Overlay.hpp"
#include "MainMenu.hpp"
#include "MapsMenu.hpp"
#include "SDLWrappers/Font.hpp"
#include "MenuBase.hpp"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    // Current menu selected
    std::shared_ptr<MenuBase> menu_;

    // Window
    Size windowSize_ = {500, 500};
    std::shared_ptr<Window> window_;
};

#endif
