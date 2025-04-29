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

void Hero::display(const Texture* target)
{
    if (!sprite_ || !shadow) return;
    
    target->blit(shadow, Point{pos_.getX() - shadow->getWidth() / 2,
                               pos_.getY() - shadow->getHeight() / 2});

    target->blit(sprite_, Point{pos_.getX() - sprite_->getWidth() / 2,
                                pos_.getY() - sprite_->getHeight() / 2});
}
