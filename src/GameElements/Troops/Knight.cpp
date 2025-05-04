#include "GameElements/Troops/Knight.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Knight::sprite_ = nullptr;

std::shared_ptr<Knight> Knight::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Knight>(lobj) : nullptr;
}

bool Knight::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Knight::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/knight.png");
}

void Knight::quit()
{
    sprite_ = nullptr;
}


Knight::Knight(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Knight::display(const std::weak_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
