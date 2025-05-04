#include "logic/units/Camp.hpp"
#include "Texture.hpp"
#include <memory>

std::shared_ptr<Texture> Camp::sprite_ = nullptr;

std::shared_ptr<Camp> Camp::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Camp>(lobj) : nullptr;
}

bool Camp::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Camp::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/camp.png");
}

void Camp::quit()
{
    sprite_ = nullptr;
}


Camp::Camp(const Point& pos): GameElement(pos, sprite_->getSize()) {}

void Camp::addCoins(int coins) {
    treasury_ += coins;
}

bool Camp::getTreasury() const {
    return treasury_;
}

void Camp::display(const std::weak_ptr<BlitTarget>& target) {
    auto ltarget = target.lock();
    if (!ltarget || !sprite_) return;

    ltarget->blit(sprite_, pos_ - sprite_->getSize() / 2);
}
