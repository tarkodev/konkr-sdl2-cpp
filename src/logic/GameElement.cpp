#include "logic/GameElement.hpp"

SDL_Renderer* GameElement::renderer = nullptr;

GameElement::GameElement(int strength, int cost, int upkeep, Player* owner)
    : strength_(strength), cost_(cost), upkeep_(upkeep), owner_(owner) {}

void GameElement::init(SDL_Renderer* renderer)
{
    GameElement::renderer = renderer;
    /* Les sprites spécifiques seront chargés par les sous-classes. */
}
