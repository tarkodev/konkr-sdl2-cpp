#include "GameElements/Town.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::shared_ptr<Texture> Town::sprite_ = nullptr;
std::shared_ptr<Texture> Town::selectSprite_ = nullptr;

std::shared_ptr<Town> Town::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Town>(lobj) : nullptr;
}

bool Town::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Town::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");

    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/builds/town.png");
    selectSprite_ = std::make_shared<Texture>(renderer_, "../assets/img/builds/bgtown.png");
}

void Town::quit()
{
    sprite_ = nullptr;
    selectSprite_ = nullptr;
}


Town::Town(const Point& pos, const int& treasury)
    : GameElement(pos, sprite_->getSize()), treasuryDisplayer_(Point{pos.getX(), pos.getY() + sprite_->getHeight() / 2}, treasury), treasury_(treasury)
{}

void Town::setPos(const Point& pos) {
    GameElement::setPos(pos);
    treasuryDisplayer_.setPos(pos);
}

int Town::getStrength() const {
    return STRENGTH;
};

int Town::getCost()     const {
    return COST;
};

int Town::getUpkeep()   const {
    return UPKEEP;
};


int Town::getTreasury() const {
    return treasury_;
}

void Town::setTreasury(int treasury) {
    treasury_ = treasury;
    treasuryDisplayer_.setTreasury(treasury_);
}

void Town::updateTreasury() {
    treasury_ += income_;
    treasuryDisplayer_.setTreasury(treasury_);
};

int Town::getIncome() const {
    return income_;
}

void Town::setIncome(int income) {
    income_ = income;
    treasuryDisplayer_.setIncome(income_);
}

void Town::addIncome(int coins) {
    income_ += coins;
    treasuryDisplayer_.setIncome(income_);
}

void Town::setSelected(bool selected) {
    selected_ = selected;
}

void Town::display(const std::weak_ptr<BlitTarget>& target) {
    auto ltarget = target.lock();
    if (!ltarget || !sprite_) return;

    if (selected_)
        ltarget->blit(selectSprite_, Point{pos_.getX()-selectSprite_->getWidth()/2, pos_.getY()-selectSprite_->getHeight()/2});

    ltarget->blit(sprite_, pos_ - sprite_->getSize() / 2);
}

void Town::displayTreasury(const std::weak_ptr<BlitTarget>& target) {
    treasuryDisplayer_.display(target);
}
