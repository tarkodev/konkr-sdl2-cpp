#include "logic/units/Pikeman.hpp"
#include "Texture.hpp"

Texture* Pikeman::sprite_ = nullptr;

void Pikeman::init()
{
    if (!renderer_)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/pikeman.png"))->convertAlpha();
}


Pikeman::Pikeman(const Point& pos): Troop(pos) {}

void Pikeman::display(const BlitTarget* target)
{
    Troop::displaySprite(target, sprite_);
}
