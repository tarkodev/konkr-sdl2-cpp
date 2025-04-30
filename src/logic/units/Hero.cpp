#include "logic/units/Hero.hpp"
#include "Texture.hpp"

Texture* Hero::sprite_ = nullptr;

void Hero::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/hero.png"))->convertAlpha();
}


Hero::Hero(const Point& pos): Troop(pos) {}

void Hero::display(const BlitTarget* target)
{
    Troop::displaySprite(target, sprite_);
}
