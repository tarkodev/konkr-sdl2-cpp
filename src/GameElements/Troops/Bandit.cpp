#include "GameElements/Troops/Bandit.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::shared_ptr<Texture> Bandit::sprite_ = nullptr;

std::shared_ptr<Bandit> Bandit::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Bandit>(lobj) : nullptr;
}

const bool Bandit::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Bandit::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/bandit.png");
}

void Bandit::quit()
{
    sprite_ = nullptr;
}


Bandit::Bandit(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Bandit::display(const std::weak_ptr<BlitTarget>& target) const {
    Troop::displaySprite(target, sprite_);
}

std::shared_ptr<GameElement> Bandit::deepCopy() const {
    auto bandit = std::make_shared<Bandit>(pos_);
    bandit->free_ = free_;
    bandit->movable_ = movable_;
    bandit->lost_ = lost_;
    return bandit;
}
