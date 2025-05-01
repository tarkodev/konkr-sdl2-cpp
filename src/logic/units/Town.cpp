#include "logic/units/Town.hpp"
#include "Texture.hpp"

Texture* Town::sprite_ = nullptr;
Texture* Town::selectSprite_ = nullptr;

void Town::init()
{
    if (!renderer_)
        std::runtime_error("GameElement not initialized");

    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/town.png"))->convertAlpha();
    selectSprite_ = (new Texture(renderer_, "../assets/img/bgtown.png"))->convertAlpha();
}


Town::Town(const Point& pos): GameElement(pos) {}

int Town::addNextCoins(int coins) {
    if (coins >= 0) {
        nextCoins_ += coins;
        return 0;
    }

    if (nextCoins_ < 0)
        return coins;

        nextCoins_ += coins;
    if (nextCoins_ < 0) {
        coins = nextCoins_;
        nextCoins_ = 0;
        return coins;
    }

    return 0;
}

void Town::display(const BlitTarget* target)
{
    if (!sprite_) return;

    if (selected_)
        target->blit(selectSprite_, Point{pos_.getX()-selectSprite_->getWidth()/2, pos_.getY()-selectSprite_->getHeight()/2});

    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2, pos_.getY()-sprite_->getHeight()/2});
}
