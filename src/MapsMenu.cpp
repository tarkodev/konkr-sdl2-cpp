#include "MapsMenu.hpp"
#include "SDL.h"
#include "MainMenu.hpp"
#include "ColorUtils.hpp"
#include "GameMenu.hpp"
#include "Checker.hpp"
#include "SDLWrappers/Cursor.hpp"
#include <filesystem>

namespace fs = std::filesystem;

MapsMenu::MapsMenu(const std::shared_ptr<Window>& window) : MenuBase{window}
{
    // 1) Récupère la liste des maps en scannant le dossier
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

    // Init pos of Map
    Point pos = window_->getSize() / 2;

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

        pos.addX(-btn.getWidth() / 2);
    }

    // Set pos of maps
    pos = window_->getSize() / 2;
    for (auto& btn : buttons_)
        pos.addX(-btn.getWidth() / 2);
    for (auto& btn : buttons_) {
        pos.addX(btn.getWidth() / 2);
        btn.setPos(pos);
        pos.addX(btn.getWidth() / 2);
    }

    // Add back button
    buttons_.emplace_back(Point{0, 0}, "../assets/img/back_btn.png", "../assets/img/back_btn_hover.png", "../assets/img/back_btn_pressed.png");
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

