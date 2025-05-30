#include "Menus/MapsMenu.hpp"
#include "SDL.h"
#include "Menus/MainMenu.hpp"
#include "Utils/ColorUtils.hpp"
#include "Menus/GameMenu.hpp"
#include "Utils/Checker.hpp"
#include "SDLWrappers/Cursor.hpp"
#include <filesystem>

namespace fs = std::filesystem;

MapsMenu::MapsMenu(const std::shared_ptr<Window>& window) : MenuBase{window}
{
    // Set filepaths
    const fs::path mapsDir   = "../assets/map/";
    const fs::path imagesDir = "../assets/img/map/";
    std::vector<std::string> mapNames;

    // Check dir exists
    if (!fs::exists(mapsDir) || !fs::is_directory(mapsDir))
        throw std::runtime_error("Dossier de maps introuvable : " + mapsDir.string());

    // Get names of files
    for (auto& entry : fs::directory_iterator(mapsDir)) {
        if (!entry.is_regular_file()) continue;
        const fs::path& path = entry.path();
        if (path.extension() != ".ascii") continue;

        auto stem = path.stem().string();
        mapNames.push_back(stem);
    }
    if (mapNames.empty())
        throw std::runtime_error("Aucune map trouvée dans " + mapsDir.string());

    // Init maps
    for (auto const& name : mapNames) {
        // File of map
        std::string mapFile = (mapsDir / (name + ".ascii")).string();

        // Verify if image exists
        fs::path imgPath = imagesDir / (name + ".png");
        if (!fs::exists(imgPath))
            imgPath = imagesDir / "unknown_map.png";

        // Create button
        buttons_.emplace_back(
            Point{0, 0},
            imgPath.string()
        );
        auto& btn = buttons_.back();

        // Callback to go to the map
        btn.setCallback([this, window, mapFile](){
            nextMenu_ = std::make_shared<GameMenu>(window, mapFile);
            loop_ = false;
        });
    }

    // Set pos of buttons
    int nbBtns = buttons_.size();
    int nbLines = std::ceil(nbBtns / 5.);
    Point pos = window_->getSize() / 2 - Point{0, nbLines * buttons_[0].getHeight() / 2};
    for (int i = 0; i < nbLines; i++) {
        pos.setX(window_->getWidth() / 2);
        pos.addY(buttons_[0].getHeight() / 2);
        for (int j = 0; j < std::min(nbBtns - i*5, 5); j++)
            pos.addX(-buttons_[i*5 + j].getWidth() / 2);
        for (int j = 0; j < std::min(nbBtns - i*5, 5); j++) {
            pos.addX(buttons_[i*5 + j].getWidth() / 2);
            buttons_[i*5 + j].setPos(pos);
            pos.addX(buttons_[i*5 + j].getWidth() / 2);
        }
        pos.addY(buttons_[0].getHeight() / 2);
    }

    // Add back button
    buttons_.emplace_back(Point{0, 0}, "../assets/img/buttons/back_btn.png", "../assets/img/buttons/back_btn_hover.png", "../assets/img/buttons/back_btn_pressed.png");
    auto& backBtn = buttons_.back();
    backBtn.setPos(Point{window_->getWidth() / 8, window_->getHeight() - backBtn.getHeight() - 10});
    backBtn.setCallback([this, window](){
        loop_ = false;
        nextMenu_ = std::make_shared<MainMenu>(window);
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

