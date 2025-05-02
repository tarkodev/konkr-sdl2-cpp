#include "logic/units/Town.hpp"
#include "Texture.hpp"

std::shared_ptr<Texture> Town::sprite_ = nullptr;
std::shared_ptr<Texture> Town::selectSprite_ = nullptr;

void Town::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");

    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/town.png");
    selectSprite_ = std::make_shared<Texture>(renderer_, "../assets/img/bgtown.png");
}


Town::Town(const Point& pos)
    : GameElement(pos, sprite_->getSize()), treasuryDisplayer_(Point{pos.getX(), pos.getY() + sprite_->getHeight() / 2})
{}


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

void Town::display(const std::shared_ptr<BlitTarget>& target)
{
    if (!sprite_) return;

    if (selected_)
        target->blit(selectSprite_, Point{pos_.getX()-selectSprite_->getWidth()/2, pos_.getY()-selectSprite_->getHeight()/2});

    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2, pos_.getY()-sprite_->getHeight()/2});
}

void Town::displayTreasury(const std::shared_ptr<BlitTarget>& target)
{
    treasuryDisplayer_.display(target);
}
