#include "Cell.hpp"

const std::string Cell::TYPE = "Cell";
const std::string Cell::getType() {
    return Cell::TYPE;
}

void Cell::setNeighbors(std::vector<Cell*> cellNeighbors) {
    neighbors = cellNeighbors;
}

const std::vector<Cell*> Cell::getNeighbors() const {
    return neighbors;
}
