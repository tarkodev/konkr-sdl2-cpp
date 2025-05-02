#include "Displayer.hpp"
#include "Point.hpp"
#include "BlitTarget.hpp"
#include "Texture.hpp"

std::shared_ptr<SDL_Renderer> Displayer::renderer_ = nullptr;

void Displayer::init(const std::shared_ptr<SDL_Renderer>& renderer) {
    renderer_ = renderer;
}

Displayer::Displayer(const Point& pos, const Size& size): pos_(pos), size_(size) {}

Point Displayer::getPos() const {
    return pos_;
}

void Displayer::setPos(const Point& pos) {
    pos_ = pos;
}

Size Displayer::getSize() const {
    return size_;
}

int Displayer::getWidth() const {
    return size_.getWidth();
}

int Displayer::getHeight() const {
    return size_.getHeight();
}
