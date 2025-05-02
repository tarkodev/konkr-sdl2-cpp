#include "logic/units/Villager.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Villager::sprite_ = nullptr;

void Villager::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");

    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/villager.png");
}


Villager::Villager(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Villager::display(const std::shared_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
