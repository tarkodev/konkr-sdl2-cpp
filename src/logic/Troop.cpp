#include "logic/Troop.hpp"

Troop::Troop(int strength, int cost, int upkeep, Player* owner)
    : GameElement(strength, cost, upkeep, owner) {}

bool Troop::moveTo(const Point& dest)
{
    // TODO : valider le déplacement (portée, obstacles, etc.)
    position_ = dest;
    return true;
}
