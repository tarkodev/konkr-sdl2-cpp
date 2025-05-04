#include "GameElements/Camp.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
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
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/builds/camp.png");
}

void Camp::quit()
{
    sprite_ = nullptr;
}


Camp::Camp(const Point& pos): 
    GameElement(pos, sprite_->getSize()), treasuryDisplayer_(Point{pos.getX(), pos.getY() + sprite_->getHeight() / 2})
{
    treasuryDisplayer_.setNoIncome(true);
}

void Camp::setPos(const Point& pos) {
    GameElement::setPos(pos);
    treasuryDisplayer_.setPos(pos);
}

void Camp::addCoins(int coins) {
    treasury_ += coins;
    treasuryDisplayer_.setTreasury(treasury_);
}

int Camp::getTreasury() const {
    return treasury_;
}

void Camp::display(const std::weak_ptr<BlitTarget>& target) {
    auto ltarget = target.lock();
    if (!ltarget || !sprite_) return;

    ltarget->blit(sprite_, pos_ - sprite_->getSize() / 2);
}

void Camp::displayTreasury(const std::weak_ptr<BlitTarget>& target) {
    treasuryDisplayer_.display(target);
}
