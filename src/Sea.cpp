#include "Sea.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

SDL_Renderer* Sea::renderer_ = nullptr;
Texture* Sea::sprite_ = nullptr;

Sea::Sea() {}

void Sea::init(SDL_Renderer* renderer, Texture* plate) {
    renderer_ = renderer;
    sprite_ = plate;
}

Texture* Sea::getSprite() {
    if (sprite_ == nullptr)
        throw std::runtime_error("Sea not initialized");

    return sprite_;
}

void Sea::handleEvents(SDL_Event &event) {}

void Sea::draw(Texture* texture, const Point& pos) {}


//! A voir si garder (gardé pour mettre dans les sous-classes)
/*
void Sea::handleEvents() {}

void Sea::draw(SDL_Point& pos) {
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