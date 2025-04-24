#include "Territory.hpp"
#include "ColorUtils.hpp"

HexagonDisplayer Territory::islandDisplayer_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
HexagonDisplayer Territory::plateDisplayer = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
double Territory::islandRadius_ = 0;
double Territory::islandInnerRadius_ = 0;

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
    islandInnerRadius_ = island->getWidth() / 2;
    islandRadius_ = islandInnerRadius_ * 2 / std::sqrt(3);

    // Set displayer of territory
    islandDisplayer_ = HexagonDisplayer{renderer_, islandRadius_, island, link, linkBottomLeft, linkBottom, linkBottomRight};


    // Load plate
    Texture* plate = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha();
    Texture* plateLink = (new Texture(renderer_, "../assets/img/plate_link.png"))->convertAlpha();
    Texture* plateLinkBottomLeft = (new Texture(renderer_, "../assets/img/plate_link_bottom_left.png"))->convertAlpha();
    Texture* plateLinkBottom = (new Texture(renderer_, "../assets/img/plate_link_bottom.png"))->convertAlpha();
    Texture* plateLinkBottomRight = (new Texture(renderer_, "../assets/img/plate_link_bottom_right.png"))->convertAlpha();
    
    plateDisplayer = HexagonDisplayer{renderer_, islandRadius_, plate, plateLink, plateLinkBottomLeft, plateLinkBottom, plateLinkBottomRight};
}

const Size Territory::getSpriteSize() {
    return islandDisplayer_.getSize();
}

const int Territory::getRadius() {
    return islandRadius_;
}

const int Territory::getInnerRadius() {
    return islandInnerRadius_;
}


Territory::Territory() {}

void Territory::display(const Texture* target, const Point& pos) {
    std::vector<bool> TerritoryNeighbors{
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[0])),
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[1])),
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[2])),
        static_cast<bool>(dynamic_cast<Territory*>(neighbors[3]))
    };

    islandDisplayer_.display(target, pos, TerritoryNeighbors);
}

const Size Territory::getSize() const {
    return getSpriteSize();
}
