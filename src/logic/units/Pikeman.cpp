#include "logic/units/Pikeman.hpp"
#include "Texture.hpp"

Texture* Pikeman::sprite_ = nullptr;

void Pikeman::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/pikeman.png"))->convertAlpha();
}


Pikeman::Pikeman(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Pikeman::display(const BlitTarget* target)
{
    Troop::displaySprite(target, sprite_);
}
