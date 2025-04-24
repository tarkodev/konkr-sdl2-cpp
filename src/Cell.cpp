#include "Cell.hpp"

SDL_Renderer* Cell::renderer_ = nullptr;

const std::string Cell::TYPE = "Cell";
const std::string Cell::getType() {
    return Cell::TYPE;
}

void Cell::init(SDL_Renderer *renderer) {
    renderer_ = renderer;
}

void Cell::setNeighbors(std::vector<Cell*> cellNeighbors) {
    neighbors = cellNeighbors;
}
