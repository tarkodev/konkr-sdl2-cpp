#include "logic/units/Castle.hpp"
#include "Texture.hpp"

Texture* Castle::sprite_ = nullptr;

void Castle::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/castle.png"))->convertAlpha();
}


void Castle::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2,
                                pos.getY()-sprite_->getHeight()/2});
}
