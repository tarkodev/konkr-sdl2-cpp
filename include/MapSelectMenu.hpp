#pragma once
#include "MenuBase.hpp"
class Game;

class MapSelectMenu : public MenuBase {
public:
    MapSelectMenu(SDL_Renderer* r, Game& app);
    ~MapSelectMenu();
    void handleEvent(const SDL_Event& e) override;
    void render(SDL_Renderer* r) const override;
private:
    SDL_Renderer* renderer_;
    Game&         app_;
};
