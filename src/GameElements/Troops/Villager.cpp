#include "GameElements/Troops/Villager.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::shared_ptr<Texture> Villager::sprite_ = nullptr;

std::shared_ptr<Villager> Villager::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Villager>(lobj) : nullptr;
}

const bool Villager::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Villager::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");

    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/villager.png");
}

void Villager::quit()
{
    sprite_ = nullptr;
}


Villager::Villager(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Villager::display(const std::weak_ptr<BlitTarget>& target) const {
    Troop::displaySprite(target, sprite_);
}

std::shared_ptr<GameElement> Villager::deepCopy() const {
    auto villager = std::make_shared<Villager>(pos_);
    villager->free_ = free_;
    villager->movable_ = movable_;
    villager->lost_ = lost_;
    return villager;
}
