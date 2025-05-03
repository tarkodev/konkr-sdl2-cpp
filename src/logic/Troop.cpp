#include "logic/Troop.hpp"
#include <memory>

std::shared_ptr<Texture> Troop::shadow_ = nullptr;
std::shared_ptr<Texture> Troop::lostSprite_ = nullptr;

std::shared_ptr<Troop> Troop::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Troop>(lobj) : nullptr;
}

bool Troop::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Troop::init() {
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");

    if (shadow_) return;
    shadow_ = std::make_shared<Texture>(renderer_, "../assets/img/shadow.png");
    lostSprite_ = std::make_shared<Texture>(renderer_, "../assets/img/lost.png");
}

void Troop::quit() {
    shadow_ = nullptr;
    lostSprite_ = nullptr;
}


Troop::Troop(const Point& pos, const Size& size) : GameElement(pos, size) {}

void Troop::displaySprite(const std::weak_ptr<BlitTarget>& target, const std::weak_ptr<Texture>& sprite) {
    auto ltarget = target.lock();
    auto lsprite = sprite.lock();
    if (!ltarget || !lsprite || !shadow_ || !lostSprite_) return;
    
    if (lost_ || free_) ltarget->blit(lostSprite_, Point{pos_.getX() - lostSprite_->getWidth() / 2, pos_.getY() - lostSprite_->getHeight() / 2});
    ltarget->blit(shadow_, Point{pos_.getX() - shadow_->getWidth() / 2, pos_.getY() - shadow_->getHeight() / 2});
    ltarget->blit(sprite, Point{pos_.getX() - lsprite->getWidth() / 2, pos_.getY() - lsprite->getHeight() / 2});
}

void Troop::setFree(bool free) {
    free_ = free;
}

bool Troop::isFree() const {
    return free_;
}
