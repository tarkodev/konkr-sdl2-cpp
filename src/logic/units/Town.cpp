#include "logic/units/Town.hpp"
#include "Texture.hpp"

Texture* Town::sprite_ = nullptr;

Town::Town(Player* owner)
    : Building(STRENGTH, COST, UPKEEP, owner) {}

void Town::onTurnStart()
{
    // TODO : ajouter les pièces à la province
}

void Town::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2,
                                pos.getY()-sprite_->getHeight()/2});
}

void Town::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");

    if (sprite_) return;
    sprite_ = new Texture(renderer, "../assets/img/town.png");
}
