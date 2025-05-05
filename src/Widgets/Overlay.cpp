#include "Widgets/Overlay.hpp"
#include "Utils/ColorUtils.hpp"
#include "GameElements/Troops/Villager.hpp"
#include "GameElements/Troops/Pikeman.hpp"
#include "GameElements/Troops/Knight.hpp"
#include "GameElements/Troops/Hero.hpp"
#include "GameElements/Castle.hpp"
#include "Widgets/GameMap.hpp"
#include "SDLWrappers/Cursor.hpp"
#include <stdexcept>

Overlay::Overlay(const Point& pos) : Displayer(pos) {
    bg_ = std::make_shared<Texture>(renderer_, "../assets/img/shop/overlay.png");
    size_ = bg_->getSize();
    bgPos_ = pos_ - size_ / 2;

    // Undo button
    undoBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/undo_btn.png", "../assets/img/buttons/undo_btn_hover.png", "../assets/img/buttons/undo_btn_pressed.png");
    undoBtn_->setCallback([this]() { undoRequested_ = true; });

    // Trun button
    turnBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/turn_btn.png", "../assets/img/buttons/turn_btn_hover.png", "../assets/img/buttons/turn_btn_pressed.png");
    turnBtn_->setCallback([this]() { turnRequested_ = true; });

    // Sprites of buyables troops
    struct Entry { const char troop; const char* noBuy; const char* buy; } spritePaths[] = {
        {'V', "../assets/img/shop/villager_no_buy.png", "../assets/img/shop/villager_buy.png"},
        {'P', "../assets/img/shop/pikeman_no_buy.png",  "../assets/img/shop/pikeman_buy.png"},
        {'K', "../assets/img/shop/knight_no_buy.png",  "../assets/img/shop/knight_buy.png"},
        {'H', "../assets/img/shop/hero_no_buy.png",    "../assets/img/shop/hero_buy.png"},
        {'C', "../assets/img/shop/castle_no_buy.png",  "../assets/img/shop/castle_buy.png"}
    };

    for (auto& path : spritePaths) {
        TroopOption opt;
        opt.texNoBuy = std::make_shared<Texture>(renderer_, path.noBuy);
        opt.btnBuy   = std::make_unique<Button>(Point{0,0}, path.buy);
        opt.btnBuy->setPressedCallback([path, this]() { buyTroopRequested_ = path.troop; });
        options_.push_back(std::move(opt));
    }
}

const bool Overlay::isHover(const Point& mousePos) const {
    // Hover of bg
    if (Rect{pos_, size_}.contains(mousePos + size_ / 2))
        return true;

    // Hover of undo/turn button
    if (undoBtn_->isHover(mousePos) || turnBtn_->isHover(mousePos))
        return true;

    // hover of troop btns
    return std::any_of(options_.begin(), options_.end(), [&](const auto& option) { return option.btnBuy && option.btnBuy->isHover(mousePos); });
}

void Overlay::setPos(const Point& pos) {
    bgPos_ = pos - size_ / 2;
    pos_ = pos;

    // Back button pos
    undoBtn_->setPos(Point{pos_.getX() - (bg_->getWidth() + undoBtn_->getWidth()) / 2, pos_.getY() + 10});

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

const bool Overlay::undoRequested() {
    auto undoRequested = undoRequested_;
    undoRequested_ = false;
    return undoRequested;
};

const bool Overlay::turnRequested() {
    auto turnRequested = turnRequested_;
    turnRequested_ = false;
    return turnRequested;
};

const bool Overlay::buyTroopRequested() {
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
    undoBtn_->handleEvent(e);
    turnBtn_->handleEvent(e);
    for (auto& opt : options_)
        if (opt.buyable)
            opt.btnBuy->handleEvent(e);
}

void Overlay::display(const std::weak_ptr<BlitTarget>& target) const {
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
        undoBtn_->display(ltarget);
        turnBtn_->display(ltarget);
    }
}