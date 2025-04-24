#include "logic/Troop.hpp"

Troupe::Troupe(int strength, int cost, int upkeep, Player* owner)
    : GameElement(strength, cost, owner),
      upkeep_(upkeep) {}

bool Troupe::moveTo(const Point& dest)
{
    // TODO : vérifier la validité du déplacement (coût, obstacles…)
    position_ = dest;
    return true;
}
