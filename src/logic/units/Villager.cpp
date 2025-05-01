#include "logic/units/Villager.hpp"
#include "Texture.hpp"

Texture* Villager::sprite_ = nullptr;

void Villager::init()
{
    if (!renderer_)
        std::runtime_error("GameElement not initialized");

    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/villager.png"))->convertAlpha();
}


Villager::Villager(const Point& pos): Troop(pos) {}

void Villager::display(const BlitTarget* target)
{
    Troop::displaySprite(target, sprite_);
}
