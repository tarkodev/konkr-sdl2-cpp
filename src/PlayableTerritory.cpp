#include "PlayableTerritory.hpp"
#include "ColorUtils.hpp"

HexagonDisplayer PlayableTerritory::plateDisplayer = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};

const std::string PlayableTerritory::TYPE = "PlayableTerritory";
const std::string PlayableTerritory::getType() {
    return PlayableTerritory::TYPE;
}

void PlayableTerritory::init() {
    if (!renderer_)
        std::runtime_error("Cell not initialized");

    // Load plate
    Texture* plate = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha();
    Texture* plateLink = (new Texture(renderer_, "../assets/img/plate_link.png"))->convertAlpha();
    Texture* plateLinkBottomLeft = (new Texture(renderer_, "../assets/img/plate_link_bottom_left.png"))->convertAlpha();
    Texture* plateLinkBottom = (new Texture(renderer_, "../assets/img/plate_link_bottom.png"))->convertAlpha();
    Texture* plateLinkBottomRight = (new Texture(renderer_, "../assets/img/plate_link_bottom_right.png"))->convertAlpha();
    
    plateDisplayer = HexagonDisplayer{renderer_, islandRadius, plate, plateLink, plateLinkBottomLeft, plateLinkBottom, plateLinkBottomRight};
}
