#include "GameElements/Player.hpp"
#include "Cells/Grounds/Ground.hpp"
#include "GameElements/Town.hpp"
#include <stdexcept>
#include <algorithm>
#include "Cells/Grounds/PlayableGround.hpp"

HexagonDisplayer Player::plateDisplayer_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
std::weak_ptr<SDL_Renderer> Player::renderer_ = {};

void Player::init(const std::shared_ptr<SDL_Renderer>& renderer) {
    renderer_ = renderer;
    if (!renderer)
        throw std::runtime_error("Renderer isn't initialized.");

    // Load plate
    std::shared_ptr<Texture> plate = std::make_shared<Texture>(renderer, "../assets/img/plate/plate.png");
    std::shared_ptr<Texture> plateLink = std::make_shared<Texture>(renderer, "../assets/img/plate/plate_link.png");
    std::shared_ptr<Texture> plateLinkBottomLeft = std::make_shared<Texture>(renderer, "../assets/img/plate/plate_link_bottom_left.png");
    std::shared_ptr<Texture> plateLinkBottom = std::make_shared<Texture>(renderer, "../assets/img/plate/plate_link_bottom.png");
    std::shared_ptr<Texture> plateLinkBottomRight = std::make_shared<Texture>(renderer, "../assets/img/plate/plate_link_bottom_right.png");

    plateDisplayer_ = HexagonDisplayer{Ground::getRadius(), plate, plateLink, plateLinkBottomLeft, plateLinkBottom, plateLinkBottomRight};
}

void Player::quit()
{
    plateDisplayer_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
}


Player::Player(const GroundColor& color) : color_(color), plate_(plateDisplayer_.copy()), lostPlate_(plateDisplayer_.copy()) {
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

void Player::addTownCell(std::weak_ptr<PlayableGround> townCell) {
    if (!townCell.expired()) {
        townCells_.push_back(townCell);
    }
}

void Player::updateTowns() {
    std::erase_if(townCells_, [](std::weak_ptr<PlayableGround> townCell) {
        auto ltownCell = townCell.lock();
        return ltownCell ? !Town::is(ltownCell->getElement()) : true;
    });
}

bool Player::hasTowns() {
    // Remove destroyed towns
    updateTowns();

    for (auto& townCell : townCells_)
        if (auto ltownCell = townCell.lock())
            if (Town::cast(ltownCell->getElement()))
                return true;

    return false;
}

std::vector<std::weak_ptr<PlayableGround>> Player::getTownCells() {
    updateTowns();
    return townCells_;
}

void Player::onTurnStart() {
    updateTowns();
    selected_ = true;
    
    for (auto& townCell : townCells_) {
        if (auto ltownCell = townCell.lock()) {
            auto town = Town::cast(ltownCell->getElement());
            town->setSelected(true);
            town->updateTreasury();
        }
    }
}

void Player::onTurnEnd() {
    // Unselect towns
    for (auto& townCell : townCells_)
        if (auto ltownCell = townCell.lock())
            if (auto town = Town::cast(ltownCell->getElement()))
                town->setSelected(false);

    selected_ = false;
}
