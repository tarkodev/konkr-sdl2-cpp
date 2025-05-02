#include "Ground.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include <memory>

HexagonDisplayer Ground::islandDisplayer = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
double Ground::islandRadius = 0;
double Ground::islandInnerRadius = 0;

const std::string Ground::TYPE = "Ground";
const std::string Ground::getType() {
    return Ground::TYPE;
}

void Ground::init() {
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");

    // Load island
    std::shared_ptr<Texture> island = std::make_shared<Texture>(renderer_, "../assets/img/hexagon.png");
    std::shared_ptr<Texture> link = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link.png");
    std::shared_ptr<Texture> linkBottomLeft = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link_bottom_left.png");
    std::shared_ptr<Texture> linkBottom = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link_bottom.png");
    std::shared_ptr<Texture> linkBottomRight = std::make_shared<Texture>(renderer_, "../assets/img/hexagon_link_bottom_right.png");

    // Get radius of hexagon of island
    islandInnerRadius = island->getWidth() / 2;
    islandRadius = HexagonUtils::innerToRadius(islandInnerRadius);

    // Set displayer of territory
    islandDisplayer = HexagonDisplayer{islandRadius, island, link, linkBottomLeft, linkBottom, linkBottomRight};
}

const Size Ground::getIslandSize() {
    return islandDisplayer.getSize();
}

const double Ground::getRadius() {
    return islandRadius;
}

const double Ground::getInnerRadius() {
    return islandInnerRadius;
}


Ground::Ground(const Point& pos): Displayer(pos, getIslandSize()) {}

void Ground::display(const std::shared_ptr<BlitTarget>& target) {
    std::vector<bool> GroundNeighbors{
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[0])),
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[1])),
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[2])),
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[3]))
    };

    islandDisplayer.display(target, pos_, GroundNeighbors);
}

