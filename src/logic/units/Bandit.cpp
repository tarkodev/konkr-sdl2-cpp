#include "logic/units/Bandit.hpp"
#include "Texture.hpp"

Texture* Bandit::sprite_ = nullptr;

void Bandit::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/bandit.png"))->convertAlpha();
}


Bandit::Bandit(const Point& pos): Troop(pos) {}

void Bandit::display(const BlitTarget* target)
{
    if (!sprite_ || !shadow) return;
    
    target->blit(shadow, Point{pos_.getX() - shadow->getWidth() / 2,
                               pos_.getY() - shadow->getHeight() / 2});

    target->blit(sprite_, Point{pos_.getX() - sprite_->getWidth() / 2,
                                pos_.getY() - sprite_->getHeight() / 2});
}
