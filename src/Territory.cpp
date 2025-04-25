#include "Territory.hpp"
#include "ColorUtils.hpp"

HexagonDisplayer Territory::islandDisplayer = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
double Territory::islandRadius = 0;
double Territory::islandInnerRadius = 0;

const std::string Territory::TYPE = "Territory";
const std::string Territory::getType() {
    return Territory::TYPE;
}

void Territory::init() {
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
    islandRadius = islandInnerRadius * 2 / std::sqrt(3);

    // Set displayer of territory
    islandDisplayer = HexagonDisplayer{renderer_, islandRadius, island, link, linkBottomLeft, linkBottom, linkBottomRight};
}

const Size Territory::getIslandSize() {
    return islandDisplayer.getSize();
}

const int Territory::getRadius() {
    return islandRadius;
}

const int Territory::getInnerRadius() {
    return islandInnerRadius;
}


void Territory::display(const Texture* target, const Point& pos) {
    std::vector<bool> TerritoryNeighbors{
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[0])),
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[1])),
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[2])),
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[3]))
    };

    islandDisplayer.display(target, pos, TerritoryNeighbors);
}

const Size Territory::getSize() const {
    return getIslandSize();
}
