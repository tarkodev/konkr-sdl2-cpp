#pragma once
#include "MenuBase.hpp"
class Game;           // forward

class MainMenu : public MenuBase {
public:
    MainMenu(Game& app);
    ~MainMenu();
    void handleEvent(const SDL_Event& e) override;

    void display(const BlitTarget* target) override;
    const Size getSize() const override { return Size{100, 100}; }; //! Changer quand MenuBase aura une size_

private:
    Game&         app_;
};
