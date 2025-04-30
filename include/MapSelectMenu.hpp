#pragma once
#include "MenuBase.hpp"
class Game;

class MapSelectMenu : public MenuBase {
public:
    MapSelectMenu(SDL_Renderer* r, Game& app);
    ~MapSelectMenu();
    void handleEvent(const SDL_Event& e) override;

    void display(const BlitTarget* target) override;
    const Size getSize() const override { return Size{100, 100}; }; //! Changer quand MenuBase aura une size_
    
private:
    SDL_Renderer* renderer_;  //! Voir s'il est nécessaire
    Game&         app_;
};
