#include "Cell.hpp"

SDL_Renderer* Cell::renderer_ = nullptr;

void Cell::init(SDL_Renderer *renderer) {
    renderer_ = renderer;
}

//! A voir si garder (gardé pour mettre dans les sous-classes)
/*
void Cell::handleEvents() {}

void Cell::draw(SDL_Point& pos) {
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