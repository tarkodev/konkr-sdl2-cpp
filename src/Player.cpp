#include "Player.hpp"
#include "Ground.hpp"
#include <stdexcept>

HexagonDisplayer Player::plateDisplayer = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
SDL_Renderer* Player::renderer_ = nullptr;

void Player::init(SDL_Renderer *renderer) {
    renderer_ = renderer;

    // Load plate
    Texture* plate = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha();
    Texture* plateLink = (new Texture(renderer_, "../assets/img/plate_link.png"))->convertAlpha();
    Texture* plateLinkBottomLeft = (new Texture(renderer_, "../assets/img/plate_link_bottom_left.png"))->convertAlpha();
    Texture* plateLinkBottom = (new Texture(renderer_, "../assets/img/plate_link_bottom.png"))->convertAlpha();
    Texture* plateLinkBottomRight = (new Texture(renderer_, "../assets/img/plate_link_bottom_right.png"))->convertAlpha();

    plateDisplayer = HexagonDisplayer{renderer_, Ground::getRadius(), plate, plateLink, plateLinkBottomLeft, plateLinkBottom, plateLinkBottomRight};
}

Player::Player(const std::string& name, const GroundColor& color) : color_(color), name_(name), plate_(plateDisplayer.copy()), lostPlate_(plateDisplayer.copy()) {
    plate_.colorize(color.owned);
    lostPlate_.colorize(color.available);
}

Player::~Player() = default;


HexagonDisplayer& Player::getPlate() {
    return plate_;
}

HexagonDisplayer& Player::getLostPlate() {
    return lostPlate_;
}
