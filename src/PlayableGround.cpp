#include "PlayableGround.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"
#include "logic/units/Castle.hpp"
#include "logic/units/Camp.hpp"
#include "logic/units/Bandit.hpp"
#include "logic/Troop.hpp"
#include <algorithm>
#include <ranges>
#include <queue>
#include <tuple>

FenceDisplayer PlayableGround::fenceDisplayer_ = FenceDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
std::vector<std::shared_ptr<Texture>> PlayableGround::shieldSprites_ = std::vector<std::shared_ptr<Texture>>();
std::shared_ptr<Texture> PlayableGround::selectableSprite_ = nullptr;


std::shared_ptr<PlayableGround> PlayableGround::cast(const std::weak_ptr<Cell>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<PlayableGround>(lobj) : nullptr;
}

bool PlayableGround::is(const std::weak_ptr<Cell>& obj) {
    return cast(obj) != nullptr;
}

void PlayableGround::init() {
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");

    // Load fence
    std::shared_ptr<Texture> fenceTop = std::make_shared<Texture>(renderer_, "../assets/img/fence_top.png");
    std::shared_ptr<Texture> fenceTopLeft = std::make_shared<Texture>(renderer_, "../assets/img/fence_top_left.png");
    std::shared_ptr<Texture> fenceTopRight = std::make_shared<Texture>(renderer_, "../assets/img/fence_top_right.png");
    std::shared_ptr<Texture> fenceBottom = std::make_shared<Texture>(renderer_, "../assets/img/fence_bottom.png");
    std::shared_ptr<Texture> fenceBottomLeft = std::make_shared<Texture>(renderer_, "../assets/img/fence_bottom_left.png");
    std::shared_ptr<Texture> fenceBottomRight = std::make_shared<Texture>(renderer_, "../assets/img/fence_bottom_right.png");
    std::shared_ptr<Texture> fenceLinkTop = std::make_shared<Texture>(renderer_, "../assets/img/fence_link_top.png");
    std::shared_ptr<Texture> fenceLinkTopLeft = std::make_shared<Texture>(renderer_, "../assets/img/fence_link_top_left.png");
    std::shared_ptr<Texture> fenceLinkTopRight = std::make_shared<Texture>(renderer_, "../assets/img/fence_link_top_right.png");
    std::shared_ptr<Texture> fenceLinkBottom = std::make_shared<Texture>(renderer_, "../assets/img/fence_link_bottom.png");
    std::shared_ptr<Texture> fenceLinkBottomLeft = std::make_shared<Texture>(renderer_, "../assets/img/fence_link_bottom_left.png");
    std::shared_ptr<Texture> fenceLinkBottomRight = std::make_shared<Texture>(renderer_, "../assets/img/fence_link_bottom_right.png");

    // Set displayer of territory
    fenceDisplayer_ = FenceDisplayer{Ground::getRadius(), 
        fenceTop, fenceTopLeft, fenceTopRight, 
        fenceBottom, fenceBottomLeft, fenceBottomRight,
        fenceLinkTop, fenceLinkTopLeft, fenceLinkTopRight,
        fenceLinkBottom, fenceLinkBottomLeft, fenceLinkBottomRight
    };

    // Load sprites of shields
    shieldSprites_.push_back(std::make_shared<Texture>(renderer_, "../assets/img/shield1.png"));
    shieldSprites_.push_back(std::make_shared<Texture>(renderer_, "../assets/img/shield2.png"));
    shieldSprites_.push_back(std::make_shared<Texture>(renderer_, "../assets/img/shield3.png"));
    shieldSprites_.push_back(std::make_shared<Texture>(renderer_, "../assets/img/shield3.png"));

    // Load selectable sprite
    selectableSprite_ = std::make_shared<Texture>(renderer_, "../assets/img/selectable.png");
    selectableSprite_->colorize(ColorUtils::YELLOW);
}

void PlayableGround::quit() {
    fenceDisplayer_ = FenceDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    shieldSprites_.clear();
    selectableSprite_ = nullptr;
}


PlayableGround::PlayableGround(const Point& pos, const std::shared_ptr<Player>& owner)
    : Ground(pos), owner_(owner)
{
    if (owner_) {
        plate_ = owner_->getPlate();
        lostPlate_ = owner_->getLostPlate();
        hasPlate_ = true;
    }
}

PlayableGround::PlayableGround(const Point& pos)
    : PlayableGround(pos, nullptr)
{}

void PlayableGround::setOwner(std::shared_ptr<Player> owner) {
    if (owner != owner_) {
        oldOwner_ = owner == nullptr ? owner_ : nullptr;
        owner_ = owner;
    }
    
    if (owner_) {
        plate_ = owner_->getPlate();
        lostPlate_ = owner_->getLostPlate();
        hasPlate_ = true;
    }
}

std::shared_ptr<Player> PlayableGround::getOwner() {
    return owner_;
}

std::shared_ptr<Player> PlayableGround::getOldOwner() {
    return oldOwner_;
}

void PlayableGround::display(const std::weak_ptr<BlitTarget>& target) {
    if (!hasPlate_) return;

    std::vector<bool> similarNeighbors;
    if (owner_) {
        for (auto& cell : neighbors_) {
            if (auto pg = PlayableGround::cast(cell))
                similarNeighbors.push_back(pg->getOwner() == owner_);
            else
                similarNeighbors.push_back(false);
        }

        plate_.display(target, pos_, similarNeighbors);

    } else {
        for (auto& cell : neighbors_) {
            if (auto pg = PlayableGround::cast(cell))
                similarNeighbors.push_back(pg->getOldOwner() == oldOwner_);
            else
                similarNeighbors.push_back(false);
        }
        
        lostPlate_.display(target, pos_, similarNeighbors);
    }
}

bool PlayableGround::hasFences() const {
    if (element && (Castle::cast(element) || Town::cast(element) || Camp::cast(element)))
        return true;

    return owner_ && std::any_of(neighbors_.begin(), neighbors_.end(), [this](auto cell) {
        if (auto pg = PlayableGround::cast(cell)) {
            if (pg->getOwner() != owner_) return false;
            auto elt = pg->getElement();
            return (elt && (Castle::cast(elt) || Town::cast(elt)));
        }
        return false;
    });
}

bool PlayableGround::isLinked(std::unordered_set<std::shared_ptr<PlayableGround>>& visited) {
    if (!owner_ || visited.find(shared_from_this()) != visited.end()) return false;
    visited.insert(shared_from_this());

    auto town = Town::cast(element);
    if (town) return true;

    for (auto& cell : neighbors_) {
        auto pg = PlayableGround::cast(cell);
        if (pg && pg->getOwner() == owner_ && pg->isLinked(visited))
            return true;
    }

    return false;
}

bool PlayableGround::isLinked() {
    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    return isLinked(visited);
}

void PlayableGround::unlink(std::unordered_set<std::shared_ptr<PlayableGround>>& visited) {
    if (visited.find(shared_from_this()) != visited.end()) return;
    visited.insert(shared_from_this());

    if (owner_) {
        for (auto& cell : neighbors_) {
            auto pg = PlayableGround::cast(cell);
            if (pg && pg->getOwner() == owner_)
                pg->unlink(visited);
        }

        setOwner(nullptr);
    }

    if (auto castle = Castle::cast(element))
        castle->lost();
    else if (auto troop = Troop::cast(element))
        troop->lost();
}

//! check si voisin.owner == null && oldowner = owner
void PlayableGround::link(const std::weak_ptr<Player>& owner, std::unordered_set<std::shared_ptr<PlayableGround>>& visited) {
    if (visited.find(shared_from_this()) != visited.end()) return;
    visited.insert(shared_from_this());

    auto lowner = owner.lock();
    if (owner_ == nullptr && lowner == oldOwner_) {
        setOwner(lowner);
        for (auto& cell : neighbors_) {
            auto pg = PlayableGround::cast(cell);
            if (pg && pg->getOldOwner() == owner_)
                pg->link(owner, visited);
        }
    } else {
        setOwner(lowner);
        for (auto& cell : neighbors_) {
            if (auto pg = PlayableGround::cast(cell)) {
                if (pg && pg->getOldOwner() == owner_)
                    pg->link(owner, visited);
                else
                    pg->updateLinked();
            }
        }
    }
}

void PlayableGround::link(const std::weak_ptr<Player>& owner) {
    auto lowner = owner.lock();
    if (!lowner || lowner == owner_) return;

    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    link(owner, visited);
}

void PlayableGround::updateLinked() {
    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    if (!isLinked(visited)) {
        visited.clear();
        unlink(visited);
    }
}

void PlayableGround::freeTroops(std::unordered_set<std::shared_ptr<PlayableGround>>& visited) {
    if (visited.find(shared_from_this()) != visited.end()) return;
    visited.insert(shared_from_this());

    if (owner_) {
        for (auto& cell : neighbors_) {
            auto pg = PlayableGround::cast(cell);
            if (pg && pg->getOwner() == owner_)
                pg->freeTroops(visited);
        }
    }

    auto troop = Troop::cast(element);
    if (troop && !Bandit::cast(troop))
        troop->setFree(true);
}

void PlayableGround::freeTroops() {
    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    freeTroops(visited);
}

// Return towns from nearest to further
void PlayableGround::getTowns(std::queue<std::weak_ptr<PlayableGround>>& toVisit, std::unordered_set<std::shared_ptr<PlayableGround>>& visited, std::vector<std::weak_ptr<Town>>& towns) {
    if (!owner_ || visited.find(shared_from_this()) != visited.end()) return;
    visited.insert(shared_from_this());

    // Add town to list
    if (auto town = Town::cast(element))
        towns.push_back(town);

    // Add neighbors to visit
    for (auto& cell : neighbors_) {
        auto ng = PlayableGround::cast(cell);
        if (ng && ng->getOwner() == owner_)
            toVisit.push(ng);
    }

    // Visit next cells
    while (!toVisit.empty()) {
        auto pg = toVisit.front();
        toVisit.pop();
        if (auto lpg = pg.lock())
            lpg->getTowns(toVisit, visited, towns);
    }
}

std::vector<std::weak_ptr<Town>> PlayableGround::getTowns() {
    std::queue<std::weak_ptr<PlayableGround>> toVisit;
    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    std::vector<std::weak_ptr<Town>> towns;

    getTowns(toVisit, visited, towns);
    return towns;
}

std::shared_ptr<Town> PlayableGround::getNearestTown(std::queue<std::weak_ptr<PlayableGround>>& toVisit, std::unordered_set<std::shared_ptr<PlayableGround>>& visited) {
    if (!owner_ || visited.find(shared_from_this()) != visited.end()) return nullptr;
    visited.insert(shared_from_this());

    // Add town to list
    if (auto town = Town::cast(element))
        return town;

    // Add neighbors to visit
    for (auto& cell : neighbors_) {
        auto ng = PlayableGround::cast(cell);
        if (ng && ng->getOwner() == owner_)
            toVisit.push(ng);
    }

    // Visit next cells
    std::shared_ptr<Town> town = nullptr;
    while (!toVisit.empty() && !town) {
        auto pg = toVisit.front();
        toVisit.pop();
        if (auto lpg = pg.lock())
            town = lpg->getNearestTown(toVisit, visited);
    }

    return town;
}

std::shared_ptr<Town> PlayableGround::getNearestTown() {
    std::queue<std::weak_ptr<PlayableGround>> toVisit;
    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    return getNearestTown(toVisit, visited);
}

void PlayableGround::updateIncome() {
    int income = element ? 1 - element->getUpkeep() : 1;
    if (income == 0) return;

    if (auto town = getNearestTown())
        town->addIncome(income);
}

void PlayableGround::displayFences(const std::weak_ptr<Texture>& target) {
    if (!hasFences()) return;

    std::vector<bool> similarNeighborsWithFences;
    if (owner_) {
        for (auto& cell : neighbors_) {
            if (auto pg = PlayableGround::cast(cell))
                similarNeighborsWithFences.push_back(pg->getOwner() == owner_ && pg->hasFences());
            else
                similarNeighborsWithFences.push_back(false);
        }
    } else similarNeighborsWithFences.resize(6, false);

    fenceDisplayer_.display(target, pos_, similarNeighborsWithFences);
}

void PlayableGround::displayElement(const std::weak_ptr<Texture>& target) {
    if (element) element->display(target);
}

void PlayableGround::displayShield(const std::weak_ptr<Texture>& target) {
    auto ltarget = target.lock();
    if (!ltarget || element) return;

    int shield = getShield();
    if (shield > 0) {
        auto shieldTex = shieldSprites_[shield - 1];

        ltarget->blit(shieldTex, Point{
            static_cast<int>(pos_.getX() - shieldTex->getWidth() / 2.0),
            static_cast<int>(pos_.getY() - shieldTex->getHeight() / 2.0)
        });
    }
}

void PlayableGround::displaySelectable(const std::weak_ptr<Texture>& target) {
    auto ltarget = target.lock();
    if (!ltarget || !selectable_ || (owner_ && owner_->hasSelected())) return;

    ltarget->blit(selectableSprite_, Point{
        static_cast<int>(pos_.getX() - selectableSprite_->getWidth() / 2.0),
        static_cast<int>(pos_.getY() - selectableSprite_->getHeight() / 2.0)
    });
}



void PlayableGround::setElement(const std::shared_ptr<GameElement>& elt) {
    element = elt;
    if (element) elt->setPos(pos_);
}

std::shared_ptr<GameElement> PlayableGround::getElement() {
    return element;
}

int PlayableGround::getShield() const {
    int maxStrength = element ? element->getStrength() : 0;

    for (auto& cell : neighbors_) {
        auto pg = PlayableGround::cast(cell);
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

void PlayableGround::updateSelectable(int strength, std::unordered_set<std::shared_ptr<PlayableGround>>& visited) {
    if (visited.find(shared_from_this()) != visited.end()) return;
    visited.insert(shared_from_this());

    selectable_ = strength > 0;
    
    for (auto& cell : neighbors_) {
        if (auto pg = PlayableGround::cast(cell)) {
            if (pg->getOwner() == owner_) pg->updateSelectable(strength, visited);
            else pg->setSelectable(pg->getShield() < strength);
        }
    }
}

void PlayableGround::updateSelectable(int strength) {
    std::unordered_set<std::shared_ptr<PlayableGround>> visited;
    updateSelectable(strength, visited);
}
