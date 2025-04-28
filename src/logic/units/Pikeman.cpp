#include "logic/units/Pikeman.hpp"
#include "Texture.hpp"

Texture* Pikeman::sprite_ = nullptr;

void Pikeman::init()
{
    if (!renderer)
        std::runtime_error("GameElement not initialized");
        
    if (sprite_) return;
    sprite_ = (new Texture(renderer, "../assets/img/pikeman.png"))->convertAlpha();
}

void Pikeman::display(const Texture* target, const Point& pos)
{
    if (!sprite_ || !shadow) return;
    
    target->blit(shadow, Point{pos.getX() - shadow->getWidth() / 2,
                               pos.getY() - shadow->getHeight() / 2});

    target->blit(sprite_, Point{pos.getX() - sprite_->getWidth() / 2,
                                pos.getY() - sprite_->getHeight() / 2});
}
