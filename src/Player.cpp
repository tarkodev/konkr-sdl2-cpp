#include "Player.hpp"
#include "Ground.hpp"
#include "logic/units/Town.hpp"
#include <stdexcept>
#include <algorithm>
#include "PlayableGround.hpp"

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
        town->updateCapital();
        //! Si un camp sur la map, le bandit doit ajouter le coin au camp (camp le plus proche)

        if (town->getCapital() < 0) {
            // townCell->freeTroops();
            town->setCapital(0);
        }
    }
}

void Player::onTurnEnd() {
    // Unselect towns
    for (PlayableGround* townCell : townCells_)
        if (Town* town = dynamic_cast<Town*>(townCell->getElement()))
            town->setSelected(false);

    selected_ = false;
}
