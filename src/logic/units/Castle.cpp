#include "logic/units/Castle.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Castle::sprite_ = nullptr;

void Castle::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/castle.png");
}


Castle::Castle(const Point& pos): GameElement(pos, sprite_->getSize()) {}

void Castle::display(const std::shared_ptr<BlitTarget>& target)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2,
                                pos_.getY()-sprite_->getHeight()/2});
}
