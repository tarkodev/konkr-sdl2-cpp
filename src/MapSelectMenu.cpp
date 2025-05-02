#include "MapSelectMenu.hpp"
#include "SDL.h"
#include "MainMenu.hpp"
#include "ColorUtils.hpp"

static const struct { const char* label; const char* file; } MAPS[] = {
    {"Ten Paces",   "../assets/map/Ten Paces.txt"},
    {"Unity",       "../assets/map/Unity.txt"}
};


//! faire fonction init pour passer le renderer
MapSelectMenu::MapSelectMenu(const std::shared_ptr<Window>& window) : MenuBase{window}
{
    int y = 120;
    for(auto map : MAPS){
        buttons_.emplace_back(Point{200, y}, std::string("../assets/map/")+map.label+".png");
        auto& btn = buttons_.back();
        btn.setCallback([this](){
            //! générer map
            loop_ = false;
        });

        y += 120;
    }

    // Back button
    buttons_.emplace_back(Point{50, 30}, "../assets/img/back.png");
    auto& backBtn = buttons_.back();
    backBtn.setCallback([this](){
        loop_ = false;
        nextMenu_ = std::make_shared<MainMenu>(window_);
    });
}

void MapSelectMenu::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        for(auto& button : buttons_)
            button.handleEvent(event);

        handleEvent(event);
    }
}

void MapSelectMenu::draw() {
    window_->fill(ColorUtils::SEABLUE);

    for(auto& button : buttons_)
        button.display(window_.get());

    window_->refresh();
}

std::shared_ptr<MenuBase> MapSelectMenu::run() {
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

