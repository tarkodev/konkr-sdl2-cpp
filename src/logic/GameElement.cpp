#include "logic/GameElement.hpp"

SDL_Renderer* GameElement::renderer = nullptr;

void GameElement::init(SDL_Renderer* renderer)
{
    GameElement::renderer = renderer;
}

GameElement::GameElement(const Point& pos): Displayer(pos) {
    if (!renderer) throw std::runtime_error("Troop not initialized");
}
