#ifndef CELL_HPP
#define CELL_HPP

#include "SDL.h"
#include <vector>
#include <string>

/* Classe théoriquement abstraite */
class Cell {
public:
    virtual ~Cell() = default;

    void setNeighbors(const std::vector<std::weak_ptr<Cell>>& neighbors);
    std::vector<std::weak_ptr<Cell>> getNeighbors() const;

protected:
    Cell() = default;
    
    std::vector<std::weak_ptr<Cell>> neighbors_;
};

#endif
