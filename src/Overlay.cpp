// Overlay.cpp
#include "Overlay.hpp"

Overlay::Overlay(const Point& pos): Displayer(pos) {
    bg_ = std::make_shared<Texture>(renderer_, "../assets/img/overlay.png");
    size_ = bg_->getSize();
    bgPos_ = pos_ - size_ / 2;

    buttons_.emplace_back(Point{150, 500}, "../assets/img/back_btn.png");
    auto& backBtn = buttons_.back();
    backBtn.setCallback([this]() {
        //! a compléter
    });

    buttons_.emplace_back(Point{150, 300}, "../assets/img/villager.png");
    auto& villagerBtn = buttons_.back();
    villagerBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    buttons_.emplace_back(Point{250, 300}, "../assets/img/pikeman.png");
    auto& pikemanBtn = buttons_.back();
    pikemanBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    buttons_.emplace_back(Point{350, 300}, "../assets/img/knight.png");
    auto& knightBtn = buttons_.back();
    knightBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    buttons_.emplace_back(Point{450,300}, "../assets/img/hero.png");
    auto& heroBtn = buttons_.back();
    heroBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    buttons_.emplace_back(Point{650,300}, "../assets/img/Castle.png");
    auto& castleBtn = buttons_.back();
    castleBtn.setCallback([]() {
        std::cout << "Next button clicked!" << std::endl;
    });

    buttons_.emplace_back(Point{150,200}, "../assets/img/turn.png");
    auto& turnBtn = buttons_.back();
    turnBtn.setCallback([]() {
        std::cout << "Turn button clicked!" << std::endl;
    });
}

void Overlay::handleEvent(const SDL_Event& e) {
    for (auto& btn : buttons_)
        btn.handleEvent(e);
}

void Overlay::display(const std::shared_ptr<BlitTarget>& target) {
    target->blit(bg_, bgPos_);
    for (auto& btn : buttons_)
        btn.display(target);
}
