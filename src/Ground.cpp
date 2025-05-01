#include "Ground.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"

HexagonDisplayer Ground::islandDisplayer = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
double Ground::islandRadius = 0;
double Ground::islandInnerRadius = 0;

const std::string Ground::TYPE = "Ground";
const std::string Ground::getType() {
    return Ground::TYPE;
}

void Ground::init() {
    if (!renderer_)
        std::runtime_error("Cell not initialized");

    // Load island
    Texture* island = (new Texture(renderer_, "../assets/img/hexagon.png"))->convertAlpha();
    Texture* link = (new Texture(renderer_, "../assets/img/hexagon_link.png"))->convertAlpha();
    Texture* linkBottomLeft = (new Texture(renderer_, "../assets/img/hexagon_link_bottom_left.png"))->convertAlpha();
    Texture* linkBottom = (new Texture(renderer_, "../assets/img/hexagon_link_bottom.png"))->convertAlpha();
    Texture* linkBottomRight = (new Texture(renderer_, "../assets/img/hexagon_link_bottom_right.png"))->convertAlpha();

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


Ground::Ground(const Point& pos): Displayer(pos) {}

void Ground::display(const BlitTarget* target) {
    std::vector<bool> GroundNeighbors{
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[0])),
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[1])),
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[2])),
        static_cast<bool>(dynamic_cast<Ground*>(neighbors[3]))
    };

    islandDisplayer.display(target, pos_, GroundNeighbors);
}

const Size Ground::getSize() const {
    return getIslandSize();
}
