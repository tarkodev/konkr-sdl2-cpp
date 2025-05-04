#ifndef GAME_HPP
#define GAME_HPP

//------------------------------
// C++ Standard Library
//------------------------------
#include <memory>               // std::shared_ptr, std::make_shared

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"                // SDL initialization, event handling, ticks

//------------------------------
// Utilities
//------------------------------
#include "Utils/ColorUtils.hpp" // Color manipulation helpers

//------------------------------
// UI Widgets
//------------------------------
#include "Widgets/GameMap.hpp"  // Main game map rendering and logic
#include "Widgets/Overlay.hpp"  // UI overlay for turn controls and purchase

//------------------------------
// Rendering Window
//------------------------------
#include "SDLWrappers/Renderers/Window.hpp" // SDL window and renderer abstraction

//------------------------------
// Game Elements
//------------------------------
#include "GameElements/Player.hpp" // Player state and turn management

//------------------------------
// Menus
//------------------------------
#include "Menus/MainMenu.hpp" // Main entry menu
#include "Menus/MapsMenu.hpp" // Map selection menu
#include "Menus/MenuBase.hpp" // Abstract base for all menus

//------------------------------
// Text Rendering
//------------------------------
#include "SDLWrappers/Font.hpp" // TTF font wrapper for UI text

/**
 * @brief Entry point for the Konkr application.
 *
 * Manages SDL initialization, top‐level menu loop, and application teardown.
 */
class Game {
public:
    /**
     * @brief Construct the Game, initialize SDL, create window, and load global resources.
     */
    Game();

    /**
     * @brief Destroy the Game, free all resources and quit SDL subsystems.
     */
    ~Game();

    /**
     * @brief Main loop: runs the current menu until the user exits.
     */
    void run();

private:
    /// Currently active menu (MainMenu, MapsMenu, GameMenu, etc.)
    std::shared_ptr<MenuBase> menu_;

    /// Initial window logical size (overridden to fullscreen desktop size)
    Size windowSize_ = {500, 500};

    /// SDL window and renderer wrapper
    std::shared_ptr<Window> window_;
};

#endif // GAME_HPP
