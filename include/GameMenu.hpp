#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include "Point.hpp"
#include "MenuBase.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Overlay.hpp"
#include "GameMap.hpp"
#include "Button.hpp"
#include <memory>
#include <string>

class MapsMenu;

class GameMenu : public MenuBase {
public:
    GameMenu(const std::shared_ptr<Window>& window, const std::string& mapPath);

    ~GameMenu() = default;

    std::shared_ptr<MenuBase> run() override;

protected:
    void handleEvents() override;
    void draw() override;

private:
    std::unique_ptr<GameMap> map_;
    std::unique_ptr<Overlay> overlay_;
    std::unique_ptr<Button> finishBtn_;
    std::shared_ptr<Texture> finishTex_;

    Size windowSize_;

    Size minHexSize_;
    Size maxHexSize_;

    Size minMapSize_;
    Size maxMapSize_;
    Size mapCellSize_;
    Point mapPos_;

    int frameCount_;
    Uint32 lastLogTime_;

    std::optional<Point> moveOrigin_;
    bool moved_ = false;
    bool gameFinished_ = false;

    void createMap(const std::string& mapPath);
    void updateMapPos();
    void updateShop();

    void onMouseButtonDown(SDL_Event& event);
    void onMouseMotion(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);
    void onMouseWheel(SDL_Event& event);
    void onKeyDown(SDL_Event& event);
    bool handleOverlay(SDL_Event& event);
};

#endif
