#include "Displayer.hpp"
#include "Point.hpp"
#include "BlitTarget.hpp"
#include "Texture.hpp"

SDL_Renderer* Displayer::renderer_ = nullptr;

void Displayer::init(SDL_Renderer *renderer) {
    renderer_ = renderer;
}

Displayer::Displayer(const Point& pos): pos_(pos) {}

Point Displayer::getPos() const {
    return pos_;
}

void Displayer::setPos(const Point& pos) {
    pos_ = pos;
}
