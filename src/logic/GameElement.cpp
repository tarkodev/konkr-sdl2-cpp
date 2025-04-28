#include "logic/GameElement.hpp"

SDL_Renderer* GameElement::renderer = nullptr;

void GameElement::init(SDL_Renderer* renderer)
{
    GameElement::renderer = renderer;
}
