#include "logic/GameElement.hpp"

GameElement::GameElement(const Point& pos, const Size& size): Displayer(pos, size) {
    if (!renderer_) throw std::runtime_error("Displayer not initialized");
}
