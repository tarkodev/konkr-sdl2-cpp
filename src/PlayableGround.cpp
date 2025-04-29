#include "PlayableGround.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"
#include "logic/units/Castle.hpp"
#include "logic/units/Camp.hpp"
#include "logic/units/Bandit.hpp"
#include "logic/Troop.hpp"
#include <algorithm>
#include <ranges>

FenceDisplayer PlayableGround::fenceDisplayer_ = FenceDisplayer{ nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
std::vector<Texture*> PlayableGround::shieldSprites_ = std::vector<Texture*>();
Texture* PlayableGround::selectableSprite_ = nullptr;

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

    // Load sprites of shields
    shieldSprites_.push_back((new Texture(renderer_, "../assets/img/shield1.png"))->convertAlpha());
    shieldSprites_.push_back((new Texture(renderer_, "../assets/img/shield2.png"))->convertAlpha());
    shieldSprites_.push_back((new Texture(renderer_, "../assets/img/shield3.png"))->convertAlpha());
    shieldSprites_.push_back((new Texture(renderer_, "../assets/img/shield3.png"))->convertAlpha());

    // Load selectable sprite
    selectableSprite_ = (new Texture(renderer_, "../assets/img/selectable.png"))->convertAlpha();
    selectableSprite_->colorize(ColorUtils::YELLOW);
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
                similarNeighbors.push_back(!(pg->getOwner()) && pg->getOldOwner() == oldOwner_);
            else
                similarNeighbors.push_back(false);
        }
        
        lostPlate_.display(target, pos, similarNeighbors);
    }
}

bool PlayableGround::hasFences() const {
    if (element && (dynamic_cast<Castle*>(element) || dynamic_cast<Town*>(element) || dynamic_cast<Camp*>(element)))
        return true;

    return owner_ && std::any_of(neighbors.begin(), neighbors.end(), [this](Cell* n) {
        if (auto* pg = dynamic_cast<PlayableGround*>(n)) {
            if (pg->getOwner() != owner_) return false;
            auto elt = pg->getElement();
            return (elt && (dynamic_cast<Castle*>(elt) || dynamic_cast<Town*>(elt)));
        }
        return false;
    });
}

bool PlayableGround::isLinked(std::unordered_set<PlayableGround*>& visited) {
    if (!owner_ || visited.find(this) != visited.end()) return false;
    visited.insert(this);

    auto town = dynamic_cast<Town*>(element);
    if (town) return true;

    for (Cell* n : neighbors) {
        auto* pg = dynamic_cast<PlayableGround*>(n);
        if (pg && pg->getOwner() == owner_ && pg->isLinked(visited))
            return true;
    }

    return false;
}

bool PlayableGround::isLinked() {
    std::unordered_set<PlayableGround*> visited;
    return isLinked(visited);
}

void PlayableGround::unlink(std::unordered_set<PlayableGround*>& visited) {
    if (visited.find(this) != visited.end()) return;
    visited.insert(this);

    if (owner_) {
        for (Cell* n : neighbors) {
            auto* pg = dynamic_cast<PlayableGround*>(n);
            if (pg && pg->getOwner() == owner_)
                pg->unlink(visited);
        }

        setOwner(nullptr);
    }

    if (auto castle = dynamic_cast<Castle*>(element)) {
        delete castle;
        element = new Camp();
        oldOwner_ = nullptr;
        hasPlate_ = false;
    } else if (auto troop = dynamic_cast<Troop*>(element)) {
        delete troop;
        element = new Bandit();
    }
}

//! check si voisin.owner == null && oldowner = owner
void PlayableGround::link(Player* owner, std::unordered_set<PlayableGround*>& visited) {
    if (visited.find(this) != visited.end()) return;
    visited.insert(this);

    if (owner_ == nullptr && owner == oldOwner_) {
        setOwner(owner);
        for (Cell* n : neighbors) {
            auto* pg = dynamic_cast<PlayableGround*>(n);
            if (pg && pg->getOwner() == nullptr && pg->getOldOwner() == owner_)
                pg->link(owner, visited);
        }
    } else {
        setOwner(owner);
        for (Cell* n : neighbors) {
            if (auto* pg = dynamic_cast<PlayableGround*>(n)) {
                //! faire en sorte que si owner, alors oldOwner est systématiquement nullptr
                if (pg && pg->getOwner() == nullptr && pg->getOldOwner() == owner_)
                    pg->link(owner, visited);
                else
                    pg->updateLinked();
            }
        }
    }
}

void PlayableGround::link(Player* owner) {
    if (owner == owner_) return;

    std::unordered_set<PlayableGround*> visited;
    link(owner, visited);
}

void PlayableGround::updateLinked() {
    std::unordered_set<PlayableGround*> visited;
    if (!isLinked(visited)) {
        visited.clear();
        unlink(visited);
    }
}

void PlayableGround::displayFences(const Texture* target, const Point& pos) {
    if (!hasFences()) return;

    std::vector<bool> similarNeighborsWithFences;
    if (owner_) {
        for (Cell* n : neighbors) {
            if (auto* pg = dynamic_cast<PlayableGround*>(n))
                similarNeighborsWithFences.push_back(pg->getOwner() == owner_ && pg->hasFences());
            else
                similarNeighborsWithFences.push_back(false);
        }
    } else similarNeighborsWithFences.resize(6, false);

    fenceDisplayer_.display(target, pos, similarNeighborsWithFences);
}

void PlayableGround::displayElement(const Texture* target, const Point& pos) {
    if (element) element->display(target, pos);
}

void PlayableGround::displayShield(const Texture* target, const Point& pos) {
    if (element) return;

    int shield = getShield();
    if (shield > 0) {
        auto shieldTex = shieldSprites_[shield - 1];

        target->blit(shieldTex, Point{
            static_cast<int>(pos.getX() - shieldTex->getWidth() / 2.0),
            static_cast<int>(pos.getY() - shieldTex->getHeight() / 2.0)
        });
    }
}

void PlayableGround::displaySelectable(const Texture* target, const Point& pos) {
    if (!selectable_ || (owner_ && owner_->hasSelected())) return;

    target->blit(selectableSprite_, Point{
        static_cast<int>(pos.getX() - selectableSprite_->getWidth() / 2.0),
        static_cast<int>(pos.getY() - selectableSprite_->getHeight() / 2.0)
    });
}



void PlayableGround::setElement(GameElement* elt) {
    element = elt;
}

GameElement* PlayableGround::getElement() {
    return element;
}

int PlayableGround::getShield() const {
    int maxStrength = element ? element->getStrength() : 0;

    for (Cell* cell : neighbors) {
        auto* pg = dynamic_cast<PlayableGround*>(cell);
        if (!pg) continue;

        auto owner = pg->getOwner();
        if (!owner || owner != owner_) continue;

        auto elt = pg->getElement();
        if (!elt) continue;
        
        maxStrength = std::max(maxStrength, elt->getStrength());
    }

    return maxStrength;
}

void PlayableGround::setSelectable(bool selectable) {
    selectable_ = selectable;
}

void PlayableGround::updateSelectable(int strength, std::unordered_set<PlayableGround*>& visited) {
    if (visited.find(this) != visited.end()) return;
    visited.insert(this);

    selectable_ = strength > 0;
    
    for (Cell* cell : neighbors) {
        if (auto* pg = dynamic_cast<PlayableGround*>(cell)) {
            if (pg->getOwner() == owner_) pg->updateSelectable(strength, visited);
            else pg->setSelectable(pg->getShield() < strength);
        }
    }
}

void PlayableGround::updateSelectable(int strength) {
    std::unordered_set<PlayableGround*> visited;
    updateSelectable(strength, visited);
}
