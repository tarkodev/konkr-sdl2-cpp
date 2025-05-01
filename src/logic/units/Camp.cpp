#include "logic/units/Camp.hpp"
#include "Texture.hpp"

Texture* Camp::sprite_ = nullptr;

void Camp::init()
{
    if (!renderer_)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/camp.png"))->convertAlpha();
}


Camp::Camp(const Point& pos): GameElement(pos) {}

void Camp::display(const BlitTarget* target)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2,
                                pos_.getY()-sprite_->getHeight()/2});
}
