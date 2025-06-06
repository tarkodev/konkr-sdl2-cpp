#include "GameElements/Troops/Pikeman.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include <memory>

std::shared_ptr<Texture> Pikeman::sprite_ = nullptr;

std::shared_ptr<Pikeman> Pikeman::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Pikeman>(lobj) : nullptr;
}

const bool Pikeman::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Pikeman::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/troops/pikeman.png");
}

void Pikeman::quit()
{
    sprite_ = nullptr;
}


Pikeman::Pikeman(const Point& pos): Troop(pos, sprite_->getSize()) {}

void Pikeman::display(const std::weak_ptr<BlitTarget>& target) const {
    Troop::displaySprite(target, sprite_);
}

std::shared_ptr<GameElement> Pikeman::deepCopy() const {
    auto pikeman = std::make_shared<Pikeman>(pos_);
    pikeman->free_ = free_;
    pikeman->movable_ = movable_;
    pikeman->lost_ = lost_;
    return pikeman;
}
