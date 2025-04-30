#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "SDL.h"
#include "ColorUtils.hpp"
#include "GameMap.hpp"
#include "Window.hpp"
#include "Player.hpp"
#include "Overlay.hpp"
#include "MainMenu.hpp"
#include "MapSelectMenu.hpp"

enum class ScreenState { MainMenu, MapSelect, InGame };

enum class PendingAction { None, OpenMapSelect, OpenMainMenu, StartGameWithMap };


class Game {
public:
    Game();
    ~Game();

    void handleEvents();
    void draw();
    void run();
    void openMapSelect();
    void openMainMenu();
    void startGameWithMap(const std::string& file);
    void requestAction(PendingAction a) { pendingAction_ = a; }   
    void requestStartGame(const std::string& file) {
        pendingMapFile_ = file;
        pendingAction_  = PendingAction::StartGameWithMap;
    }
    
    

private:
    double hexSize_ = 100;
    bool moved_ = false;

    // Init size of window size
    Size windowSize_ = {500, 500};

    // Map size and pos (in px)
    Point mapPos_ = {0, 0};
    Size mapCellSize_ = {0, 0};

    // Grid size
    std::pair<int, int> gridSize_ = {20, 15};

    // Loop condition of game
    bool loop_ = true;

    Size minHexSize_;
    Size maxHexSize_;
    Size minMapSize_;
    Size maxMapSize_;

    std::optional<Point> moveOrigin_;

    // Background color
    const SDL_Color bgColor = ColorUtils::SEABLUE;


    // Object to render textures
    SDL_Renderer* renderer_;

    // Window
    std::unique_ptr<Window> window_;

    // Map of game
    std::optional<GameMap> map_;

    // Overlay
    Overlay overlay_;

    // Screen
    ScreenState screen_ {ScreenState::MainMenu};
    std::unique_ptr<class MenuBase> activeMenu_;   // pointeur polymorphique
    std::string nextMapFile_;                      // choisi dans MapSelect

    PendingAction pendingAction_;

    std::string pendingMapFile_;  
    
    
    void updateMapPos();


};

#endif
