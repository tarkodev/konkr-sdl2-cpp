#include "logic/units/Bandit.hpp"
#include "Texture.hpp"

Texture* Bandit::sprite_ = nullptr;
Size     Bandit::spriteSize_{8,8};

Bandit::Bandit()
    : Troop(STRENGTH, COST, UPKEEP, nullptr) {}

void Bandit::display(const Texture* target, const Point& pos)
{
    if (!sprite_) return;
    target->blit(sprite_, Point{pos.getX()-sprite_->getWidth()/2,
                                pos.getY()-sprite_->getHeight()/2});
}

void Bandit::init(SDL_Renderer* renderer)
{
    if (sprite_) return;
    sprite_ = new Texture(renderer, "../assets/img/bandit.png");
    spriteSize_ = sprite_->getSize();
}
