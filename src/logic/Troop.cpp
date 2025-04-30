#include "logic/Troop.hpp"

Texture* Troop::shadow_ = nullptr;
Texture* Troop::lostSprite_ = nullptr;

void Troop::init() {
    if (!renderer)
        std::runtime_error("GameElement not initialized");

    if (shadow_) return;
    shadow_ = (new Texture(renderer, "../assets/img/shadow.png"))->convertAlpha();
    lostSprite_ = (new Texture(renderer, "../assets/img/town.png"))->convertAlpha();
}


Troop::Troop(const Point& pos) : GameElement(pos) {}

void Troop::displaySprite(const BlitTarget* target, const Texture* sprite)
{
    if (!sprite || !shadow_ || !lostSprite_) return;
    
    if (lost_) target->blit(lostSprite_, Point{pos_.getX() - lostSprite_->getWidth() / 2, pos_.getY() - lostSprite_->getHeight() / 2});
    target->blit(shadow_, Point{pos_.getX() - shadow_->getWidth() / 2, pos_.getY() - shadow_->getHeight() / 2});
    target->blit(sprite, Point{pos_.getX() - sprite->getWidth() / 2, pos_.getY() - sprite->getHeight() / 2});
}
