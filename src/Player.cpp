#include "Player.hpp"
#include "ColorUtils.hpp"
#include <stdexcept>

SDL_Renderer* Player::renderer_ = nullptr;
Texture* Player::sprite_ = nullptr;

Player::Player(SDL_Color color) : color_(color)
{
    if (!plate_)
        throw std::runtime_error("La texture par défaut n'est pas chargée.");

    plate_ = sprite_->copy();
    plate_->colorize(color);
}

Player::~Player() = default;

void Player::init(SDL_Renderer* renderer, Texture* plate) {
    renderer_ = renderer;
    sprite_ = plate;
}

Texture* Player::getHexagonSprite() const {
    return plate_;
}
