#include "logic/units/Villager.hpp"
#include "ColorUtils.hpp"

Villager::Villager(Player* owner)
    : Troop(STRENGTH, COST, UPKEEP, owner) {}

void Villager::display(const Texture* target, const Point& pos)
{
    // TODO : remplacer par sprite dédié
    //SDL_Color c = owner_ ? owner_->getColor() : ColorUtils::WHITE;
    //SDL_SetRenderDrawColor(target->get()->renderer, c.r,c.g,c.b,c.a);
    //SDL_Rect r{pos.getX()-4, pos.getY()-4, 8, 8};
    //SDL_RenderFillRect(target->get()->renderer, &r);
}
