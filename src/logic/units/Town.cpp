#include "logic/units/Town.hpp"
#include "ColorUtils.hpp"

Town::Town(Player* owner)
    : Building(STRENGTH, COST, owner) {}

void Town::onTurnStart()
{
    // TODO : ajouter les revenus à la province / joueur
}

void Town::display(const Texture* target, const Point& pos)
{
    // TODO sprite
    //SDL_Color c = owner_ ? owner_->getColor() : ColorUtils::YELLOW;
    //SDL_SetRenderDrawColor(target->get()->renderer, c.r,c.g,c.b,c.a);
    //SDL_Rect r{pos.getX()-6, pos.getY()-6, 12, 12};
    //SDL_RenderFillRect(target->get()->renderer, &r);
}
