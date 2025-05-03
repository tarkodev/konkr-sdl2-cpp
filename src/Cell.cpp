#include "Cell.hpp"

void Cell::setNeighbors(const std::vector<std::weak_ptr<Cell>>& cellNeighbors) {
    neighbors_ = cellNeighbors;
}

std::vector<std::weak_ptr<Cell>> Cell::getNeighbors() const {
    return neighbors_;
}
