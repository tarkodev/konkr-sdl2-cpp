#include "Displayers/Displayer.hpp"
#include "SDLWrappers/Coords/Point.hpp"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::weak_ptr<SDL_Renderer> Displayer::renderer_ = {};

void Displayer::init(const std::shared_ptr<SDL_Renderer>& renderer) {
    renderer_ = renderer;
}


Displayer::Displayer(const Point& pos, const Size& size): pos_(pos), size_(size) {}

const Point Displayer::getPos() const {
    return pos_;
}

void Displayer::setPos(const Point& pos) {
    pos_ = pos;
}

Size Displayer::getSize() const {
    return size_;
}

const int Displayer::getWidth() const {
    return size_.getWidth();
}

const int Displayer::getHeight() const {
    return size_.getHeight();
}
