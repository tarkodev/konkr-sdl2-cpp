#include "Plain.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

Texture* Plain::sprite_ = nullptr;

Plain::Plain() {}

void Plain::init() {
    if (!Territory::sprite_)
        std::runtime_error("Territory not initialized");
        
    sprite_ = Territory::sprite_->copy();
    sprite_->colorize(ColorUtils::GREEN);
}

void Plain::handleEvents(SDL_Event &event) {

}

void Plain::draw(Texture* texture, const Point& pos) {
    texture->blit(sprite_, Point{
        static_cast<int>(pos.getX() - sprite_->getWidth() / 2.0),
        static_cast<int>(pos.getY() - sprite_->getHeight() / 2.0)
    });
}


//! A voir si garder (gardé pour mettre dans les sous-classes)
/*
void Plain::handleEvents() {}

void Plain::draw(SDL_Point& pos) {
    SDL_Texture* sprite = owner_ ? owner_->getHexagonSprite() : sprite_;
    SDL_Rect size = owner_ ? owner_->getHexagonSize() : spriteSize_;

    SDL_Rect dest = {
        pos.x,
        pos.y,
        size.w,
        size.h
    };

    SDL_RenderCopy(renderer_, sprite, &size, &dest);
}
*/