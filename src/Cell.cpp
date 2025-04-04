#include "Cell.hpp"

SDL_Texture* Cell::defaultSprite_ = nullptr;
SDL_Rect Cell::spriteSize_ = {0, 0, 0, 0};

Cell::Cell(SDL_Renderer *renderer, Player* owner) : renderer_(renderer), owner_(owner) {}

void Cell::setOwner(Player* owner) {
    owner_ = owner;
}

void Cell::handleEvents() {}

void Cell::draw(SDL_Point& pos) {
    SDL_Texture* sprite = owner_ ? owner_->getHexagonSprite() : defaultSprite_;
    SDL_Rect size = owner_ ? owner_->getHexagonSize() : spriteSize_;

    SDL_Rect dest = {
        pos.x,
        pos.y,
        size.w,
        size.h
    };

    SDL_RenderCopy(renderer_, sprite, &size, &dest);
}
