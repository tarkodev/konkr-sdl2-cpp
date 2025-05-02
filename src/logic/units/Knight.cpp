#include "logic/units/Knight.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Knight::sprite_ = nullptr;

void Knight::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/knight.png");
}


Knight::Knight(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Knight::display(const std::shared_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
