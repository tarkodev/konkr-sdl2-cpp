#ifndef CELL_HPP
#define CELL_HPP

#include "SDL.h"
#include <vector>
#include <string>

/* Classe théoriquement abstraite */
class Cell {
public:
    static const std::string TYPE;
    static void init(SDL_Renderer *renderer);

    virtual const std::string getType();
    virtual ~Cell() = default;

    void setNeighbors(std::vector<Cell*> neighbors);

protected:
    Cell() = default;
    
    static SDL_Renderer* renderer_;
    
    std::vector<Cell*> neighbors{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
};

#endif
