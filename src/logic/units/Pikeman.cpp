#include "logic/units/Pikeman.hpp"
#include "Texture.hpp"

/* --- ressources statiques --- */
Texture* Pikeman::sprite_ = nullptr;
Size     Pikeman::spriteSize_{8, 8};

Pikeman::Pikeman(Player* owner)
    : Troop(STRENGTH, COST, UPKEEP, owner) {}

void Pikeman::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_,
                 Point{pos.getX() - sprite_->getWidth()  / 2,
                       pos.getY() - sprite_->getHeight() / 2});
}

void Pikeman::init(SDL_Renderer* renderer)
{
    if (sprite_) return;                            // déjà chargé
    sprite_     = new Texture(renderer, "../assets/img/pikeman.png");
    spriteSize_ = sprite_->getSize();
}
