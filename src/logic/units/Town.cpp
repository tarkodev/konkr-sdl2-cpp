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


void Town::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;

    if (selected_)
        target->blit(selectSprite_, Point{pos.getX()-selectSprite_->getWidth()/2, pos.getY()-selectSprite_->getHeight()/2});

    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2, pos.getY()-sprite_->getHeight()/2});
}
