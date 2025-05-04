#include "GameElements/Troops/Troop.hpp"
#include <memory>
#include <cmath>

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
    shadow_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/shadow.png");
    lostSprite_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/lost.png");
}

void Troop::quit() {
    shadow_ = nullptr;
    lostSprite_ = nullptr;
}


Troop::Troop(const Point& pos, const Size& size) : GameElement(pos, size) {}

void Troop::setFree(const bool& free) {
    free_ = free;
}

bool Troop::isFree() const {
    return free_;
}

void Troop::setMovable(const bool& movable) {
    movable_ = movable;
}

bool Troop::isMovable() const {
    return movable_;
}

void Troop::displaySprite(const std::weak_ptr<BlitTarget>& target, const std::weak_ptr<Texture>& sprite) {
    auto ltarget = target.lock();
    auto lsprite = sprite.lock();
    if (!ltarget || !lsprite || !shadow_ || !lostSprite_) return;
    
    // Display troop
    int offsetY = movable_ ? static_cast<int>((1 - std::pow((static_cast<int>(SDL_GetTicks64()) % 500 - 250) / 250., 2)) * lsprite->getHeight() / 2) : 0;
    if (lost_ || free_) ltarget->blit(lostSprite_, pos_ - lostSprite_->getSize() / 2 - Point{0, offsetY});
    ltarget->blit(shadow_, pos_ - shadow_->getSize() / 2);
    ltarget->blit(sprite, pos_ - lsprite->getSize() / 2 - Point{0, offsetY});
}
