#include "logic/units/Knight.hpp"
#include "Texture.hpp"

Texture* Knight::sprite_ = nullptr;

void Knight::init()
{
    if (!renderer_)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/knight.png"))->convertAlpha();
}


Knight::Knight(const Point& pos): Troop(pos) {}

void Knight::display(const BlitTarget* target)
{
    Troop::displaySprite(target, sprite_);
}
