#include "MapsMenu.hpp"
#include "SDL.h"
#include "MainMenu.hpp"
#include "ColorUtils.hpp"
#include "GameMenu.hpp"
#include "Checker.hpp"
#include "Cursor.hpp"

static const struct { const char* label; const char* file; } MAPS[] = {
    {"Ten Paces",   "../assets/map/Ten Paces.txt"},
    {"Unity",       "../assets/map/Unity.txt"}
};

MapsMenu::MapsMenu(const std::shared_ptr<Window>& window) : MenuBase{window}
{
    // Create map buttons
    Point pos = window_->getSize() / 2;
    for (auto& map : MAPS){
        buttons_.emplace_back(Point{0, 0}, std::string("../assets/map/")+map.label+".png");
        auto& btn = buttons_.back();
        btn.setCallback([this, map](){
            nextMenu_ = std::make_shared<GameMenu>(window_, map.file);
            loop_ = false;
        });

        pos.addX(-btn.getWidth() / 2);
    }

    // Set pos of buttons
    for (auto& button : buttons_) {
        int cbtn = button.getWidth() / 2;
        pos.addX(cbtn);
        button.setPos(pos);
        pos.addX(cbtn);
    }

    // Back button
    buttons_.emplace_back(Point{0, 0}, "../assets/img/back_btn.png", "../assets/img/back_btn_hover.png", "../assets/img/back_btn_pressed.png");
    auto& backBtn = buttons_.back();
    backBtn.setPos(Point{window_->getWidth() / 8, window_->getHeight() - backBtn.getHeight()});
    backBtn.setCallback([this](){
        loop_ = false;
        nextMenu_ = std::make_shared<MainMenu>(window_);
    });
}

void MapsMenu::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        for (auto& button : buttons_)
            button.handleEvent(event);

        // Go to main menu if escape pressed
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            nextMenu_ = std::make_shared<MainMenu>(window_);
            loop_ = false;
        }
        handleEvent(event);
    }

    Cursor::update();
}

void MapsMenu::draw() {
    window_->fill(ColorUtils::SEABLUE);

    for (auto& button : buttons_)
        button.display(window_);

    window_->refresh();
}

std::shared_ptr<MenuBase> MapsMenu::run() {
    loop_ = true;

    while (loop_) {
        // Handle events
        handleEvents();

        // Draw elements
        draw();

        // Control loop duration
        SDL_Delay(1/60);
    }

    return nextMenu_;
}

