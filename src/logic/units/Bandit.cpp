#include "logic/units/Bandit.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Bandit::sprite_ = nullptr;

void Bandit::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/bandit.png");
}


Bandit::Bandit(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Bandit::display(const std::shared_ptr<BlitTarget>& target)
{
    Troop::displaySprite(target, sprite_);
}
