#pragma once
#include "MenuBase.hpp"
class Game;

class MapSelectMenu : public MenuBase {
public:
    MapSelectMenu(Game& app);
    ~MapSelectMenu();
    void handleEvent(const SDL_Event& e) override;

    void display(const BlitTarget* target) override;
    const Size getSize() const override { return Size{100, 100}; }; //! Changer quand MenuBase aura une size_
    
private:
    Game&         app_;
};
