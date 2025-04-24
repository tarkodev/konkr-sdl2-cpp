#include "logic/GameElement.hpp"

GameElement::GameElement(int strength, int cost, Player* owner)
    : strength_(strength), cost_(cost), owner_(owner) {}
