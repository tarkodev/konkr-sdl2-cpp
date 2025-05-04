#include "GameElements/GameElement.hpp"

GameElement::GameElement(const Point& pos, const Size& size): Displayer(pos, size) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer)
        throw std::runtime_error("Displayer not initialized");
}
