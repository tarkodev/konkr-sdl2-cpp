#include "logic/units/Pikeman.hpp"
#include "Texture.hpp"
#include <memory>

std::shared_ptr<Texture> Pikeman::sprite_ = nullptr;

void Pikeman::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/pikeman.png");
}


Pikeman::Pikeman(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Pikeman::display(const std::shared_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
