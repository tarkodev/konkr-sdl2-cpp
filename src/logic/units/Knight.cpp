#include "logic/units/Knight.hpp"
#include "Texture.hpp"

Texture* Knight::sprite_ = nullptr;

void Knight::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/knight.png"))->convertAlpha();
}


Knight::Knight(const Point& pos): Troop(pos) {}

void Knight::display(const Texture* target)
{
    if (!sprite_ || !shadow) return;
    
    target->blit(shadow, Point{pos_.getX() - shadow->getWidth() / 2,
                               pos_.getY() - shadow->getHeight() / 2});

    target->blit(sprite_, Point{pos_.getX() - sprite_->getWidth() / 2,
                                pos_.getY() - sprite_->getHeight() / 2});
}
