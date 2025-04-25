#include "logic/units/Camp.hpp"
#include "Texture.hpp"

Texture* Camp::sprite_ = nullptr;

Camp::Camp()
    : Building(STRENGTH, COST, UPKEEP, nullptr) {}

void Camp::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2,
                                pos.getY()-sprite_->getHeight()/2});
}

void Camp::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = new Texture(renderer, "../assets/img/camp.png");
}
