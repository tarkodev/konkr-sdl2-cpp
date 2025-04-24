#include "Plain.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

HexagonDisplayer Plain::displayer = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};

const std::string Plain::TYPE = "Plain";
const std::string Plain::getType() {
    return Plain::TYPE;
}

Plain::Plain() {}

void Plain::init() {
    displayer = plateDisplayer.copy();
    displayer.colorize(ColorUtils::GREEN);
}

void Plain::display(const Texture* target, const Point& pos) {
    std::vector<bool> PlainNeighbors{
        neighbors[0] && neighbors[0]->getType() == Plain::TYPE,
        neighbors[1] && neighbors[1]->getType() == Plain::TYPE,
        neighbors[2] && neighbors[2]->getType() == Plain::TYPE,
        neighbors[3] && neighbors[3]->getType() == Plain::TYPE
    };

    displayer.display(target, pos, PlainNeighbors);
}
