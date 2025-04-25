#include "logic/units/Knight.hpp"
#include "Texture.hpp"

Texture* Knight::sprite_ = nullptr;
Size     Knight::spriteSize_{10, 10};

Knight::Knight(Player* owner)
    : Troop(STRENGTH, COST, UPKEEP, owner) {}

void Knight::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_,
                 Point{pos.getX() - sprite_->getWidth()  / 2,
                       pos.getY() - sprite_->getHeight() / 2});
}

void Knight::init(SDL_Renderer* renderer)
{
    if (sprite_) return;
    sprite_     = new Texture(renderer, "../assets/img/knight.png");
    spriteSize_ = sprite_->getSize();
}
