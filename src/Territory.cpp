#include "Territory.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

Texture* Territory::sprite_ = nullptr;

Territory::Territory() {}

void Territory::init() {
    if (!renderer_)
        std::runtime_error("Cell not initialized");

    sprite_ = new Texture(renderer_, "../assets/img/plate.png");
    sprite_->convertAlpha();
}

void Territory::handleEvents(SDL_Event &event) {

}

void Territory::draw(Texture* texture, const Point& pos) {}


//! A voir si garder (gardé pour mettre dans les sous-classes)
/*
void Territory::handleEvents() {}

void Territory::draw(SDL_Point& pos) {
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