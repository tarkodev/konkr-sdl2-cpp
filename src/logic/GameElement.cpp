#include "logic/GameElement.hpp"

GameElement::GameElement(const Point& pos): Displayer(pos) {
    if (!renderer_) throw std::runtime_error("Displayer not initialized");
}
