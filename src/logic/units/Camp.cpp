#include "logic/units/Camp.hpp"
#include "Texture.hpp"
#include <memory>

std::shared_ptr<Texture> Camp::sprite_ = nullptr;

void Camp::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/camp.png");
}


Camp::Camp(const Point& pos): GameElement(pos, sprite_->getSize()) {}

void Camp::addCoins(int coins) {
    treasury_ += coins;
}

void Camp::display(const std::shared_ptr<BlitTarget>& target)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2,
                                pos_.getY()-sprite_->getHeight()/2});
}
