#pragma once
#include "MenuBase.hpp"
class Game;           // forward

class MainMenu : public MenuBase {
public:
    MainMenu(SDL_Renderer* r, Game& app);
    ~MainMenu();
    void handleEvent(const SDL_Event& e) override;
    void render(SDL_Renderer* r) const override;
private:
    SDL_Renderer* renderer_;
    Game&         app_;
};
