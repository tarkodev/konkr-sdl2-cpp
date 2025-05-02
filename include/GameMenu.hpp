#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include "Point.hpp"
#include "MenuBase.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Overlay.hpp"
#include "GameMap.hpp"
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

    Size windowSize_;

    Size minHexSize_;
    Size maxHexSize_;

    Size minMapSize_;
    Size maxMapSize_;
    Size mapCellSize_;
    Point mapPos_;

    std::optional<Point> moveOrigin_;
    bool moved_ = false;

    void createMap(const std::string& mapPath);
    void updateMapPos();

    void onMouseButtonDown(SDL_Event& event);
    void onMouseMotion(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);
    void onMouseWheel(SDL_Event& event);
    void onKeyDown(SDL_Event& event);
};

#endif
