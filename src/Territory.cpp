#include "Territory.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

SDL_Renderer* Territory::renderer_ = nullptr;
Texture* Territory::sprite_ = nullptr;

Territory::Territory() {}

void Territory::init(SDL_Renderer* renderer, Texture* plate) {
    renderer_ = renderer;
    sprite_ = plate;
}

Texture* Territory::getSprite() {
    if (sprite_ == nullptr)
        throw std::runtime_error("Territory not initialized");

    return sprite_;
}

void Territory::handleEvents(SDL_Event &event) {

}

void Territory::draw(Point& pos) {
    Size spriteSize = sprite_->getSize();
    Rect dest = {pos, spriteSize};

    SDL_RenderCopy(renderer_, sprite_->get(), nullptr, &dest.get());
}


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