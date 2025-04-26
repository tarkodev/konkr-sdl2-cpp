#include "PlayableGround.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

const std::string PlayableGround::TYPE = "PlayableGround";
const std::string PlayableGround::getType() {
    return PlayableGround::TYPE;
}

PlayableGround::PlayableGround(Player *owner)
    : owner_(owner)
{
    if (owner_) {
        plate_ = owner_->getPlate();
        hasPlate_ = true;
    }
}

PlayableGround::PlayableGround()
    : PlayableGround(nullptr)
{}

void PlayableGround::setOwner(Player *owner) {
    owner_ = owner;
    if (owner_) {
        plate_ = owner_->getPlate();
        hasPlate_ = true;
    }
}

Player* PlayableGround::getOwner() {
    return owner_;
}

void PlayableGround::display(const Texture* target, const Point& pos) {
    std::vector<bool> PlayableGroundNeighbors{
        neighbors[0] && neighbors[0]->getType() == PlayableGround::TYPE && dynamic_cast<PlayableGround*>(neighbors[0])->getOwner() == getOwner(),
        neighbors[1] && neighbors[1]->getType() == PlayableGround::TYPE && dynamic_cast<PlayableGround*>(neighbors[1])->getOwner() == getOwner(),
        neighbors[2] && neighbors[2]->getType() == PlayableGround::TYPE && dynamic_cast<PlayableGround*>(neighbors[2])->getOwner() == getOwner(),
        neighbors[3] && neighbors[3]->getType() == PlayableGround::TYPE && dynamic_cast<PlayableGround*>(neighbors[3])->getOwner() == getOwner()
    };

    if (hasPlate_)
        plate_.display(target, pos, PlayableGroundNeighbors);
}

void PlayableGround::setElement(GameElement* elt) {
    element = elt;
}

GameElement* PlayableGround::getElement() {
    return element;
}