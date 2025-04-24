#include "PlayerTerritory.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

const std::string PlayerTerritory::TYPE = "PlayerTerritory";
const std::string PlayerTerritory::getType() {
    return PlayerTerritory::TYPE;
}

PlayerTerritory::PlayerTerritory(Player *owner)
    : owner_(owner)
{
    if (owner_ && !owner->hasPlate()) {
        HexagonDisplayer plate = plateDisplayer.copy();
        plate.colorize(owner->getColor());
        owner_->setPlateDisplayer(plate);
    }
}

void PlayerTerritory::init() {
}

void PlayerTerritory::setOwner(Player *owner) {
    owner_ = owner;
    if (owner_ && !owner->hasPlate()) {
        HexagonDisplayer plate = plateDisplayer.copy();
        plate.colorize(owner->getColor());
        owner_->setPlateDisplayer(plate);
    }
}

Player* PlayerTerritory::getOwner() {
    return owner_;
}

void PlayerTerritory::display(const Texture* target, const Point& pos) {
    if (!owner_) return;

    std::vector<bool> PlayerTerritoryNeighbors{
        neighbors[0] && neighbors[0]->getType() == PlayerTerritory::TYPE && dynamic_cast<PlayerTerritory*>(neighbors[0])->getOwner() == getOwner(),
        neighbors[1] && neighbors[1]->getType() == PlayerTerritory::TYPE && dynamic_cast<PlayerTerritory*>(neighbors[1])->getOwner() == getOwner(),
        neighbors[2] && neighbors[2]->getType() == PlayerTerritory::TYPE && dynamic_cast<PlayerTerritory*>(neighbors[2])->getOwner() == getOwner(),
        neighbors[3] && neighbors[3]->getType() == PlayerTerritory::TYPE && dynamic_cast<PlayerTerritory*>(neighbors[3])->getOwner() == getOwner()
    };

    owner_->getPlateDisplayer().display(target, pos, PlayerTerritoryNeighbors);
}
