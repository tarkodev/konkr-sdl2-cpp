#include "logic/Troop.hpp"

//! peut être faire un troopDisplayer (héritant de Displayer)
Texture* Troop::shadow = nullptr;

Troop::Troop(int strength, int cost, int upkeep, Player* owner)
    : GameElement(strength, cost, upkeep, owner) {}

bool Troop::moveTo(const Point& dest)
{
    // TODO : valider le déplacement (portée, obstacles, etc.)
    position_ = dest;
    return true;
}

void Troop::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");

    if (shadow) return;
    shadow = new Texture(renderer, "../assets/img/shadow.png");
}
