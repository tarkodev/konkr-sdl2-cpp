#include "logic/units/Villager.hpp"
#include "Texture.hpp"

Texture* Villager::sprite_ = nullptr;
Size     Villager::spriteSize_{8,8};

Villager::Villager(Player* owner)
    : Troop(STRENGTH, COST, UPKEEP, owner) {}

void Villager::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2,
                                pos.getY()-sprite_->getHeight()/2});
}

void Villager::init(SDL_Renderer* renderer)
{
    if (sprite_) return;
    sprite_ = new Texture(renderer, "../assets/img/villager.png");
    spriteSize_ = sprite_->getSize();
}
