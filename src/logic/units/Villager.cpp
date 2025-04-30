#include "logic/units/Villager.hpp"
#include "Texture.hpp"

Texture* Villager::sprite_ = nullptr;

void Villager::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");

    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/villager.png"))->convertAlpha();
}


Villager::Villager(const Point& pos): Troop(pos) {}

void Villager::display(const BlitTarget* target)
{
    if (!sprite_ || !shadow) return;
    
    target->blit(shadow, Point{pos_.getX() - shadow->getWidth() / 2,
                               pos_.getY() - shadow->getHeight() / 2});

    target->blit(sprite_, Point{pos_.getX() - sprite_->getWidth() / 2,
                                pos_.getY() - sprite_->getHeight() / 2});
}
