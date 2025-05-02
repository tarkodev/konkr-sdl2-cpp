#include "Player.hpp"
#include "Ground.hpp"
#include "logic/units/Town.hpp"
#include <stdexcept>
#include <algorithm>
#include "PlayableGround.hpp"

HexagonDisplayer Player::plateDisplayer = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
std::shared_ptr<SDL_Renderer> Player::renderer_ = nullptr;

void Player::init(const std::shared_ptr<SDL_Renderer>& renderer) {
    renderer_ = renderer;

    // Load plate
    std::shared_ptr<Texture> plate = std::make_shared<Texture>(renderer_, "../assets/img/plate.png");
    std::shared_ptr<Texture> plateLink = std::make_shared<Texture>(renderer_, "../assets/img/plate_link.png");
    std::shared_ptr<Texture> plateLinkBottomLeft = std::make_shared<Texture>(renderer_, "../assets/img/plate_link_bottom_left.png");
    std::shared_ptr<Texture> plateLinkBottom = std::make_shared<Texture>(renderer_, "../assets/img/plate_link_bottom.png");
    std::shared_ptr<Texture> plateLinkBottomRight = std::make_shared<Texture>(renderer_, "../assets/img/plate_link_bottom_right.png");

    plateDisplayer = HexagonDisplayer{Ground::getRadius(), plate, plateLink, plateLinkBottomLeft, plateLinkBottom, plateLinkBottomRight};
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

void Player::addTownCell(PlayableGround* townCell) {
    if (townCell) townCells_.insert(townCell);
}

void Player::updateTowns() {
    std::erase_if(townCells_, [](PlayableGround* townCell) {
        return dynamic_cast<Town*>(townCell->getElement()) == nullptr;
    });
}

bool Player::hasTowns() {
    // Remove destroyed towns
    updateTowns();

    for (PlayableGround* townCell : townCells_)
        if (dynamic_cast<Town*>(townCell->getElement()))
            return true;

    return false;
}

std::unordered_set<PlayableGround*> Player::getTownCells() {
    updateTowns();
    return townCells_;
}

void Player::onTurnStart() {
    updateTowns();
    selected_ = true;
    
    for (PlayableGround* townCell : townCells_) {
        Town* town = dynamic_cast<Town*>(townCell->getElement());
        town->setSelected(true);
        town->updateTreasury();
    }
}

void Player::onTurnEnd() {
    // Unselect towns
    for (PlayableGround* townCell : townCells_)
        if (Town* town = dynamic_cast<Town*>(townCell->getElement()))
            town->setSelected(false);

    selected_ = false;
}
