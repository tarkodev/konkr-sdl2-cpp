#include "SDL.h"
#include "Utils/ColorUtils.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/MapsMenu.hpp"
#include "Menus/MakeMenu.hpp"
#include "Utils/Checker.hpp"
#include "SDLWrappers/Cursor.hpp"

MainMenu::MainMenu(const std::shared_ptr<Window>& window): MenuBase{window} {
    // Background of menu
    bg_ = std::make_shared<Texture>(window_->getRenderer(), "../assets/img/main_bg.png");
    
    // Logo
    logo_ = std::make_shared<Texture>(window_->getRenderer(), "../assets/img/logo.png");

    // Expedition button
    expeditionBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/expeditions.png", "../assets/img/buttons/expeditions_hover.png");
    expeditionBtn_->setCallback([this]() {
        loop_ = false;
        nextMenu_ = std::make_shared<MapsMenu>(window_);
    });

    // How to play button
    howToPlayBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/howtoplay.png", "../assets/img/buttons/howtoplay_hover.png");
    howToPlayBtn_->setCallback([]() {
        SDL_Check(SDL_OpenURL("https://www.konkr.io/how-to-play/"), "SDL_OpenURL");
    });

    // Quit button
    exitBtn_ = std::make_unique<Button>(Point{0, 0}, "../assets/img/buttons/exit.png", "../assets/img/buttons/exit_hover.png");
    exitBtn_->setCallback([this]() {
        loop_ = false;
    });

    Size center{static_cast<int>(window_->getWidth() * 0.75), static_cast<int>(window_->getHeight() / 2)};
    double gap = howToPlayBtn_->getHeight() * 0.75;
    expeditionBtn_->setPos(center - Point{0, static_cast<int>(gap + expeditionBtn_->getHeight() / 2)});
    howToPlayBtn_->setPos(center);
    exitBtn_->setPos(center + Point{0, static_cast<int>(gap + exitBtn_->getHeight() / 2)});
    logoPos_ = expeditionBtn_->getPos() - Point{static_cast<int>(logo_->getWidth() / 2), expeditionBtn_->getHeight() + logo_->getHeight()};
}

void MainMenu::handleEvents(){
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        expeditionBtn_->handleEvent(event);
        howToPlayBtn_->handleEvent(event);
        exitBtn_->handleEvent(event);

        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_m) {
            nextMenu_ =  std::make_shared<MakeMenu>(window_);;
            loop_ = false;
        }

        handleEvent(event);
    }

    Cursor::update();
}

void MainMenu::draw() {
    window_->fill(ColorUtils::SEABLUE);

    window_->blit(bg_);
    window_->blit(logo_, logoPos_);
    expeditionBtn_->display(window_);
    howToPlayBtn_->display(window_);
    exitBtn_->display(window_);

    window_->refresh();
}

std::shared_ptr<MenuBase> MainMenu::run() {
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
