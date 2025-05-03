// Overlay.cpp
#include "Overlay.hpp"

Overlay::Overlay(const Point& pos): Displayer(pos) {
    bg_ = std::make_shared<Texture>(renderer_, "../assets/img/overlay.png");
    size_ = bg_->getSize();
    bgPos_ = pos_ - size_ / 2;

    backBtn_ = std::make_unique<Button>(Point{150, 500}, "../assets/img/back_btn.png");
    backBtn_->setCallback([this]() {
        //! a compléter
    });

    troopBtns_.emplace_back(Point{150, 300}, "../assets/img/villager.png");
    auto& villagerBtn = troopBtns_.back();
    villagerBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    troopBtns_.emplace_back(Point{250, 300}, "../assets/img/pikeman.png");
    auto& pikemanBtn = troopBtns_.back();
    pikemanBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    troopBtns_.emplace_back(Point{350, 300}, "../assets/img/knight.png");
    auto& knightBtn = troopBtns_.back();
    knightBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    troopBtns_.emplace_back(Point{450,300}, "../assets/img/hero.png");
    auto& heroBtn = troopBtns_.back();
    heroBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    troopBtns_.emplace_back(Point{650,300}, "../assets/img/Castle.png");
    auto& castleBtn = troopBtns_.back();
    castleBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    turnBtn_ = std::make_unique<Button>(Point{150,200}, "../assets/img/turn.png");
    turnBtn_->setCallback([]() {
        std::cout << "Turn button clicked!" << std::endl;
    });
}

void Overlay::setPos(const Point& pos) {
    pos_ = pos;
    bgPos_ = pos_ - size_ / 2;

    double gap = static_cast<double>(size_.getWidth()) / troopBtns_.size();
    double x = pos_.getX() - size_.getWidth() / 2.0 + troopBtns_[0].getWidth() / 2;
    for (auto& btn : troopBtns_) {
        btn.setPos(Point{static_cast<int>(x), pos_.getY() - 50});
        x += gap;
    }

    backBtn_->setPos(Point{pos_.getX() - (bg_->getWidth() + backBtn_->getWidth()) / 2, pos_.getY()});
}

void Overlay::handleEvent(const SDL_Event& e) {
    backBtn_->handleEvent(e);
    turnBtn_->handleEvent(e);
    for (auto& btn : troopBtns_)
        btn.handleEvent(e);
}

void Overlay::display(const std::weak_ptr<BlitTarget>& target) {
    if (auto ltarget = target.lock())
        ltarget->blit(bg_, bgPos_);
        
    backBtn_->display(target);
    turnBtn_->display(target);
    for (auto& btn : troopBtns_)
        btn.display(target);
}
