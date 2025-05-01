#include "logic/units/Town.hpp"
#include "Texture.hpp"

Texture* Town::sprite_ = nullptr;
Texture* Town::selectSprite_ = nullptr;

void Town::init()
{
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");

    if (sprite_) return;
    sprite_ = (new Texture(renderer_, "../assets/img/town.png"))->convertAlpha();
    selectSprite_ = (new Texture(renderer_, "../assets/img/bgtown.png"))->convertAlpha();
}


Town::Town(const Point& pos): GameElement(pos) {
    Point treasuryPos{
        pos_.getX(),
        pos_.getY() + sprite_->getHeight() / 2
    };
    treasuryDisplayer_ = std::make_unique<TreasuryDisplayer>(treasuryPos, 0, 0);
}


const Size Town::getSize() const {
    return sprite_->getSize();
};

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
    treasuryDisplayer_->setTreasury(treasury_);
}

void Town::updateTreasury() {
    treasury_ += income_;
    treasuryDisplayer_->setTreasury(treasury_);
};

int Town::getIncome() const {
    return income_;
}

void Town::setIncome(int income) {
    income_ = income;
    treasuryDisplayer_->setIncome(income_);
}

void Town::addIncome(int coins) {
    income_ += coins;
    treasuryDisplayer_->setIncome(income_);
}

void Town::setSelected(bool selected) {
    selected_ = selected;
}

void Town::display(const BlitTarget* target)
{
    if (!sprite_) return;

    if (selected_)
        target->blit(selectSprite_, Point{pos_.getX()-selectSprite_->getWidth()/2, pos_.getY()-selectSprite_->getHeight()/2});

    target->blit(sprite_, Point{pos_.getX()-sprite_->getWidth()/2, pos_.getY()-sprite_->getHeight()/2});
}

void Town::displayTreasury(const BlitTarget* target)
{
    treasuryDisplayer_->display(target);
}
