#include "logic/Troop.hpp"

Texture* Troop::shadow = nullptr;

void Troop::init() {
    if (!renderer)
        std::runtime_error("GameElement not initialized");

    if (shadow) return;
        shadow = new Texture(renderer, "../assets/img/shadow.png");
}
