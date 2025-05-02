#include "logic/Troop.hpp"
#include <memory>

std::shared_ptr<Texture> Troop::shadow_ = nullptr;
std::shared_ptr<Texture> Troop::lostSprite_ = nullptr;

void Troop::init() {
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");

    if (shadow_) return;
    shadow_ = std::make_shared<Texture>(renderer_, "../assets/img/shadow.png");
    lostSprite_ = std::make_shared<Texture>(renderer_, "../assets/img/lost.png");
}


Troop::Troop(const Point& pos, const Size& size) : GameElement(pos, size) {}

void Troop::displaySprite(const std::shared_ptr<BlitTarget>& target, const std::shared_ptr<Texture>& sprite)
{
    if (!sprite || !shadow_ || !lostSprite_) return;
    
    if (lost_ || free_) target->blit(lostSprite_, Point{pos_.getX() - lostSprite_->getWidth() / 2, pos_.getY() - lostSprite_->getHeight() / 2});
    target->blit(shadow_, Point{pos_.getX() - shadow_->getWidth() / 2, pos_.getY() - shadow_->getHeight() / 2});
    target->blit(sprite, Point{pos_.getX() - sprite->getWidth() / 2, pos_.getY() - sprite->getHeight() / 2});
}

void Troop::setFree(bool free) {
    free_ = free;
}

bool Troop::isFree() const {
    return free_;
}
