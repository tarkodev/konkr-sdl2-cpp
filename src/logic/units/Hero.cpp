#include "logic/units/Hero.hpp"
#include "Texture.hpp"

Texture* Hero::sprite_ = nullptr;
Size     Hero::spriteSize_{12, 12};

Hero::Hero(Player* owner)
    : Troop(STRENGTH, COST, UPKEEP, owner) {}

void Hero::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_,
                 Point{pos.getX() - sprite_->getWidth()  / 2,
                       pos.getY() - sprite_->getHeight() / 2});
}

void Hero::init(SDL_Renderer* renderer)
{
    if (sprite_) return;
    sprite_     = new Texture(renderer, "../assets/img/hero.png");
    spriteSize_ = sprite_->getSize();
}
