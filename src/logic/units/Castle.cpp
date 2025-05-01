#include "logic/units/Castle.hpp"
#include "Texture.hpp"

Texture* Castle::sprite_ = nullptr;

void Castle::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/castle.png"))->convertAlpha();
}


Castle::Castle(const Point& pos): GameElement(pos) {}

void Castle::display(const BlitTarget* target)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2,
                                pos_.getY()-sprite_->getHeight()/2});
}
