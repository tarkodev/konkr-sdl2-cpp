#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "SDL.h"
#include "Utils/ColorUtils.hpp"
#include "Widgets/GameMap.hpp"
#include "SDLWrappers/Renderers/Window.hpp"
#include "GameElements/Player.hpp"
#include "Widgets/Overlay.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/MapsMenu.hpp"
#include "SDLWrappers/Font.hpp"
#include "Menus/MenuBase.hpp"

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
