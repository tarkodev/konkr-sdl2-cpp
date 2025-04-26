#include "PlayableGround.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

FenceDisplayer PlayableGround::fenceDisplayer_ = FenceDisplayer{ nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

const std::string PlayableGround::TYPE = "PlayableGround";
const std::string PlayableGround::getType() {
    return PlayableGround::TYPE;
}

void PlayableGround::init() {
    if (!renderer_)
        throw std::runtime_error("Cell not initialized");

    // Load fence
    Texture* fenceTop = (new Texture(renderer_, "../assets/img/fence_top.png"))->convertAlpha();
    Texture* fenceTopLeft = (new Texture(renderer_, "../assets/img/fence_top_left.png"))->convertAlpha();
    Texture* fenceTopRight = (new Texture(renderer_, "../assets/img/fence_top_right.png"))->convertAlpha();
    Texture* fenceBottom = (new Texture(renderer_, "../assets/img/fence_bottom.png"))->convertAlpha();
    Texture* fenceBottomLeft = (new Texture(renderer_, "../assets/img/fence_bottom_left.png"))->convertAlpha();
    Texture* fenceBottomRight = (new Texture(renderer_, "../assets/img/fence_bottom_right.png"))->convertAlpha();
    Texture* fenceLinkTop = (new Texture(renderer_, "../assets/img/fence_link_top.png"))->convertAlpha();
    Texture* fenceLinkTopLeft = (new Texture(renderer_, "../assets/img/fence_link_top_left.png"))->convertAlpha();
    Texture* fenceLinkTopRight = (new Texture(renderer_, "../assets/img/fence_link_top_right.png"))->convertAlpha();
    Texture* fenceLinkBottom = (new Texture(renderer_, "../assets/img/fence_link_bottom.png"))->convertAlpha();
    Texture* fenceLinkBottomLeft = (new Texture(renderer_, "../assets/img/fence_link_bottom_left.png"))->convertAlpha();
    Texture* fenceLinkBottomRight = (new Texture(renderer_, "../assets/img/fence_link_bottom_right.png"))->convertAlpha();

    // Set displayer of territory
    fenceDisplayer_ = FenceDisplayer{renderer_, Ground::getRadius(), 
        fenceTop, fenceTopLeft, fenceTopRight, 
        fenceBottom, fenceBottomLeft, fenceBottomRight,
        fenceLinkTop, fenceLinkTopLeft, fenceLinkTopRight,
        fenceLinkBottom, fenceLinkBottomLeft, fenceLinkBottomRight
    };
}

PlayableGround::PlayableGround(Player *owner)
    : owner_(owner)
{
    if (owner_) {
        plate_ = owner_->getPlate();
        lostPlate_ = owner_->getLostPlate();
        hasPlate_ = true;
    }
}

PlayableGround::PlayableGround()
    : PlayableGround(nullptr)
{}

void PlayableGround::setOwner(Player *owner) {
    if (owner != owner_) {
        oldOwner_ = owner_;
        owner_ = owner;
    }
    
    if (owner_) {
        plate_ = owner_->getPlate();
        lostPlate_ = owner_->getLostPlate();
        hasPlate_ = true;
    }
}

Player* PlayableGround::getOwner() {
    return owner_;
}

Player* PlayableGround::getOldOwner() {
    return oldOwner_;
}

void PlayableGround::display(const Texture* target, const Point& pos) {
    if (!hasPlate_) return;

    std::vector<bool> similarNeighbors;
    if (owner_) {
        for (Cell* n : neighbors) {
            if (auto* pg = dynamic_cast<PlayableGround*>(n))
                similarNeighbors.push_back(pg->getOwner() == owner_);
            else
                similarNeighbors.push_back(false);
        }

        plate_.display(target, pos, similarNeighbors);

    } else {
        for (Cell* n : neighbors) {
            if (auto* pg = dynamic_cast<PlayableGround*>(n))
                similarNeighbors.push_back(pg->getOldOwner() == oldOwner_);
            else
                similarNeighbors.push_back(false);
        }
        
        lostPlate_.display(target, pos, similarNeighbors);
    }
}

void PlayableGround::displayFences(const Texture* target, const Point& pos) {
    if (!hasPlate_ || !owner_) return;

    std::vector<bool> similarNeighbors;
    for (Cell* n : neighbors) {
        if (auto* pg = dynamic_cast<PlayableGround*>(n))
            similarNeighbors.push_back(pg->getOwner() == owner_);
        else
            similarNeighbors.push_back(false);
    }

    fenceDisplayer_.display(target, pos, similarNeighbors);
}

void PlayableGround::setElement(GameElement* elt) {
    element = elt;
}

GameElement* PlayableGround::getElement() {
    return element;
}
