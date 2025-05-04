#include "GameElements/Troops/Villager.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Villager::sprite_ = nullptr;

std::shared_ptr<Villager> Villager::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Villager>(lobj) : nullptr;
}

bool Villager::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Villager::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");

    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/villager.png");
}

void Villager::quit()
{
    sprite_ = nullptr;
}


Villager::Villager(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Villager::display(const std::weak_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
