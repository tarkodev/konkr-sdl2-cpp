#include "logic/units/Town.hpp"
#include "Texture.hpp"

Texture* Town::sprite_ = nullptr;
Texture* Town::selectSprite_ = nullptr;

void Town::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");

    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/town.png"))->convertAlpha();
    selectSprite_ = (new Texture(renderer, "../assets/img/bgtown.png"))->convertAlpha();
}


Town::Town(const Point& pos): GameElement(pos) {}

void Town::display(const Texture* target)
{
    if (!sprite_) return;

    if (selected_)
        target->blit(selectSprite_, Point{pos_.getX()-selectSprite_->getWidth()/2, pos_.getY()-selectSprite_->getHeight()/2});

    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2, pos_.getY()-sprite_->getHeight()/2});
}
