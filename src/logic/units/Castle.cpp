#include "logic/units/Castle.hpp"
#include "Texture.hpp"

Texture* Castle::sprite_ = nullptr;
Size     Castle::spriteSize_{12,12};

Castle::Castle(Player* owner)
    : Building(STRENGTH, COST, UPKEEP, owner) {}

void Castle::onTurnStart()
{
    // TODO : ajouter les pièces à la province
}

void Castle::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2,
                                pos.getY()-sprite_->getHeight()/2});
}

void Castle::init(SDL_Renderer* renderer)
{
    if (sprite_) return;
    sprite_ = new Texture(renderer, "../assets/img/castle.png");
    spriteSize_ = sprite_->getSize();
}
