#include "GameElements/Troops/Hero.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::shared_ptr<Texture> Hero::sprite_ = nullptr;

std::shared_ptr<Hero> Hero::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Hero>(lobj) : nullptr;
}

const bool Hero::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Hero::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/hero.png");
}

void Hero::quit()
{
    sprite_ = nullptr;
}


Hero::Hero(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Hero::display(const std::weak_ptr<BlitTarget>& target) const {
    Troop::displaySprite(target, sprite_);
}

std::shared_ptr<GameElement> Hero::deepCopy() const {
    auto hero = std::make_shared<Hero>(pos_);
    hero->free_ = free_;
    hero->movable_ = movable_;
    hero->lost_ = lost_;
    return hero;
}
