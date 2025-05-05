#ifndef CELL_HPP
#define CELL_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"            // Provides basic SDL types (e.g., SDL_Point)

//------------------------------
// Standard Library
//------------------------------
#include <vector>           // std::vector for storing neighbors
#include <string>           // (optional) for future extensions
#include <memory>           // std::weak_ptr, std::shared_ptr

/**
 * @brief Abstract base class for a grid cell.
 *
 * Represents a single cell in the game grid. Manages adjacency (neighbors)
 * via weak pointers to avoid ownership cycles.
 */
class Cell {
public:
    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~Cell() = default;

    /**
     * @brief Set the adjacent neighbor cells.
     * @param neighbors A vector of weak pointers to neighboring Cell instances.
     */
    void setNeighbors(const std::vector<std::weak_ptr<Cell>>& neighbors);

    /**
     * @brief Retrieve the current neighbor cells.
     * @return Vector of weak pointers to adjacent Cell instances.
     */
    std::vector<std::weak_ptr<Cell>> getNeighbors() const;

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    virtual std::shared_ptr<Cell> deepCopy() const = 0;

protected:
    /**
     * @brief Protected default constructor to prevent direct instantiation.
     * Derived classes must call this.
     */
    Cell() = default;

    /// Storage for adjacent cells (order defined by grid layout).
    std::vector<std::weak_ptr<Cell>> neighbors_;
};

#endif // CELL_HPP
