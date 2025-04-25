#include "logic/units/Knight.hpp"
#include "Texture.hpp"

Texture* Knight::sprite_ = nullptr;

Knight::Knight(Player* owner)
    : Troop(STRENGTH, COST, UPKEEP, owner) {}

void Knight::display(const Texture* target, const Point& pos)
{
    if (!sprite_ || !shadow) return;
    
    target->blit(shadow, Point{pos.getX() - shadow->getWidth() / 2,
                               pos.getY() - shadow->getHeight() / 2});

    target->blit(sprite_, Point{pos.getX() - sprite_->getWidth() / 2,
                                pos.getY() - sprite_->getHeight() / 2});
}

void Knight::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_     = new Texture(renderer, "../assets/img/knight.png");
}
