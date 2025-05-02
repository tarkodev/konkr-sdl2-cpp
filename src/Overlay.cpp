// Overlay.cpp
#include "Overlay.hpp"

Overlay::Overlay(): Displayer() {} //! a compléter

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

void Overlay::display(const BlitTarget* target) {
    for (auto btn : buttons_) btn->display(target);
}
