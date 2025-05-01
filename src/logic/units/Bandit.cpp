#include "logic/units/Bandit.hpp"
#include "Texture.hpp"

Texture* Bandit::sprite_ = nullptr;

void Bandit::init()
{
    if (!renderer_)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/bandit.png"))->convertAlpha();
}


Bandit::Bandit(const Point& pos): Troop(pos) {}

void Bandit::display(const BlitTarget* target)
{
    Troop::displaySprite(target, sprite_);
}
