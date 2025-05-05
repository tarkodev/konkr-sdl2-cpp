#include "GameElements/Troops/Knight.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::shared_ptr<Texture> Knight::sprite_ = nullptr;

std::shared_ptr<Knight> Knight::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Knight>(lobj) : nullptr;
}

const bool Knight::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Knight::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/knight.png");
}

void Knight::quit()
{
    sprite_ = nullptr;
}


Knight::Knight(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Knight::display(const std::weak_ptr<BlitTarget>& target) const {
    Troop::displaySprite(target, sprite_);
}

std::shared_ptr<GameElement> Knight::deepCopy() const {
    auto knight = std::make_shared<Knight>(pos_);
    knight->free_ = free_;
    knight->movable_ = movable_;
    knight->lost_ = lost_;
    return knight;
}
