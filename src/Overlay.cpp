// Overlay.cpp
#include "Overlay.hpp"

Overlay::Overlay() = default;

Overlay::~Overlay() {
    // Supprime tous les boutons ajoutés
    for (auto btn : buttons_) {
        delete btn;
    }
}

void Overlay::addButton(Button* button) {
    buttons_.push_back(button);
}

void Overlay::handleEvent(const SDL_Event& e) {
    for (auto btn : buttons_) {
        btn->handleEvent(e);
    }
}

void Overlay::render(SDL_Renderer* renderer) const {
    for (auto btn : buttons_) {
        btn->render(renderer);
    }
}
