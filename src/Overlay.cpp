#include "Overlay.hpp"
#include "ColorUtils.hpp"
#include "logic/units/Villager.hpp"
#include "logic/units/Pikeman.hpp"
#include "logic/units/Knight.hpp"
#include "logic/units/Hero.hpp"
#include "logic/units/Castle.hpp"
#include "GameMap.hpp"
#include "Cursor.hpp"
#include <stdexcept>

Overlay::Overlay(const Point& pos) : Displayer(pos) {
    bg_ = std::make_shared<Texture>(renderer_, "../assets/img/overlay.png");
    size_ = bg_->getSize();
    bgPos_ = pos_ - size_ / 2;

    // Back button
    backBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/back_btn.png", "../assets/img/back_btn_hover.png", "../assets/img/back_btn_pressed.png");
    backBtn_->setCallback([this]() { backRequested_ = true; });

    // Trun button
    turnBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/turn_btn.png", "../assets/img/turn_btn_hover.png", "../assets/img/turn_btn_pressed.png");
    turnBtn_->setCallback([this]() { turnRequested_ = true; });

    // Sprites of buyables troops
    struct Entry { const char troop; const char* noBuy; const char* buy; } spritePaths[] = {
        {'V', "../assets/img/villager_no_buy.png", "../assets/img/villager_buy.png"},
        {'P', "../assets/img/pikeman_no_buy.png",  "../assets/img/pikeman_buy.png"},
        {'K', "../assets/img/knight_no_buy.png",  "../assets/img/knight_buy.png"},
        {'H', "../assets/img/hero_no_buy.png",    "../assets/img/hero_buy.png"},
        {'C', "../assets/img/castle_no_buy.png",  "../assets/img/castle_buy.png"}
    };

    for (auto& path : spritePaths) {
        TroopOption opt;
        opt.texNoBuy = std::make_shared<Texture>(renderer_, path.noBuy);
        opt.btnBuy   = std::make_unique<Button>(Point{0,0}, path.buy);
        opt.btnBuy->setPressedCallback([path, this]() { buyTroopRequested_ = path.troop; });
        options_.push_back(std::move(opt));
    }
}

bool Overlay::isHover(const Point& mousePos) const {
    // Hover of bg
    if (Rect{pos_, size_}.contains(mousePos + size_ / 2))
        return true;

    // Hover of back/turn button
    if (backBtn_->isHover(mousePos) || turnBtn_->isHover(mousePos))
        return true;

    // hover of troop btns
    return std::any_of(options_.begin(), options_.end(), [&](const TroopOption& option) { return option.btnBuy && option.btnBuy->isHover(mousePos); });
}

void Overlay::setPos(const Point& pos) {
    bgPos_ = pos - size_ / 2;
    pos_ = pos;

    // Back button pos
    backBtn_->setPos(Point{pos_.getX() - (bg_->getWidth() + backBtn_->getWidth()) / 2, pos_.getY() + 10});

    // Turn button pos
    turnBtn_->setPos(Point{pos_.getX() + (bg_->getWidth() + turnBtn_->getWidth()) / 2, pos_.getY() + 10});

    // Position of troops
    double gap = static_cast<double>(size_.getWidth()) / (options_.size() + 1);
    double x = bgPos_.getX() + gap;
    for (auto& opt : options_) {
        Point texPos{ static_cast<int>(x), pos_.getY() - 20 };
        Point btnPos{ static_cast<int>(x), pos_.getY() - 20 };
        opt.texPos = texPos;
        opt.btnBuy->setPos(btnPos);
        x += gap;
    }
}

std::shared_ptr<GameElement> Overlay::getTroopBought() {
    if (buyTroopRequested()) {
        auto elt = GameMap::createGameElement(buyTroopRequested_, Cursor::getPos());
        buyTroopRequested_ = '\0';
        return elt;
    }

    return std::shared_ptr<GameElement>(nullptr);
}

bool Overlay::backRequested() {
    auto backRequested = backRequested_;
    backRequested_ = false;
    return backRequested;
};

bool Overlay::turnRequested() {
    auto turnRequested = turnRequested_;
    turnRequested_ = false;
    return turnRequested;
};

bool Overlay::buyTroopRequested() {
    return buyTroopRequested_ != '\0';
};

void Overlay::update(int treasury) {
    options_[0].buyable = treasury >= 10;
    options_[1].buyable = treasury >= 20;
    options_[2].buyable = treasury >= 40;
    options_[3].buyable = treasury >= 80;
    options_[4].buyable = treasury >= 20;
}

void Overlay::handleEvent(const SDL_Event& e) {
    backBtn_->handleEvent(e);
    turnBtn_->handleEvent(e);
    for (auto& opt : options_)
        if (opt.buyable)
            opt.btnBuy->handleEvent(e);
}

void Overlay::display(const std::weak_ptr<BlitTarget>& target) {
    if (auto ltarget = target.lock()) {
        // Fond
        ltarget->blit(bg_, bgPos_);

        // Options
        for (auto& opt : options_) {
            if (opt.buyable)
                opt.btnBuy->display(ltarget);
            else
                ltarget->blit(opt.texNoBuy, opt.texPos - opt.getSize() / 2);
        }

        // Buttons
        backBtn_->display(ltarget);
        turnBtn_->display(ltarget);
    }
}