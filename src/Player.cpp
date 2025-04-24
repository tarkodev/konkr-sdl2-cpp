#include "Player.hpp"
#include "ColorUtils.hpp"
#include <stdexcept>

Player::Player(const std::string& name, const SDL_Color& color) : color_(color), name_(name) {}

Player::~Player() = default;


SDL_Color& Player::getColor() {
    return color_;
}

HexagonDisplayer& Player::getPlateDisplayer() {
    return plateDisplayer_;
}

void Player::setPlateDisplayer(HexagonDisplayer& plateDisplayer) {
    plateDisplayer_ = plateDisplayer;
    hasPlate_ = true;
}

bool Player::hasPlate() const {
    return hasPlate_;
}
