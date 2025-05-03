#include "Ground.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include <memory>

HexagonDisplayer Ground::islandDisplayer_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
double Ground::islandRadius_ = 0;
double Ground::islandInnerRadius_ = 0;


std::shared_ptr<Ground> Ground::cast(const std::weak_ptr<Cell>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Ground>(lobj) : nullptr;
}

bool Ground::is(const std::weak_ptr<Cell>& obj) {
    return cast(obj) != nullptr;
}

void Ground::init() {
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");

    // Load island
    std::shared_ptr<Texture> island = std::make_shared<Texture>(renderer_, "../assets/img/hexagon.png");
    std::shared_ptr<Texture> link = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link.png");
    std::shared_ptr<Texture> linkBottomLeft = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link_bottom_left.png");
    std::shared_ptr<Texture> linkBottom = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link_bottom.png");
    std::shared_ptr<Texture> linkBottomRight = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link_bottom_right.png");

    // Get radius of hexagon of island
    islandInnerRadius_ = island->getWidth() / 2;
    islandRadius_ = HexagonUtils::innerToRadius(islandInnerRadius_);

    // Set displayer of territory
    islandDisplayer_ = HexagonDisplayer{islandRadius_, island, link, linkBottomLeft, linkBottom, linkBottomRight};
}

void Ground::quit() {
    islandDisplayer_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
}

const Size Ground::getIslandSize() {
    return islandDisplayer_.getSize();
}

const double Ground::getRadius() {
    return islandRadius_;
}

const double Ground::getInnerRadius() {
    return islandInnerRadius_;
}


Ground::Ground(const Point& pos): Displayer(pos, getIslandSize()) {}

void Ground::display(const std::weak_ptr<BlitTarget>& target) {
    if (auto ltarget = target.lock()) {
        std::vector<bool> GroundNeighbors{
            Ground::is(neighbors_[0]),
            Ground::is(neighbors_[1]),
            Ground::is(neighbors_[2]),
            Ground::is(neighbors_[3])
        };

        islandDisplayer_.display(ltarget, pos_, GroundNeighbors);
    }
}

