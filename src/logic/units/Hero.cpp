#include "logic/units/Hero.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Hero::sprite_ = nullptr;

void Hero::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/hero.png");
}


Hero::Hero(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Hero::display(const std::shared_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
