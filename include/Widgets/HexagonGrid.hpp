#ifndef HEXAGONGRID_HPP
#define HEXAGONGRID_HPP

//------------------------------
// Standard Library
//------------------------------
#include <cmath>         // std::round, std::floor for coordinate math
#include <utility>       // std::pair for grid dimensions
#include <vector>        // std::vector to store cells in contiguous memory
#include <stdexcept>     // std::out_of_range for boundary checks

/**
 * @brief Generic hexagonal grid container.
 *
 * Manages grid dimensions and storage (using an STL container),
 * and provides conversion utilities between hex coordinates and pixel positions.
 *
 * @tparam T Type of each cell in the grid.
 */
template<typename T>
class HexagonGrid {
public:
    /// Alias for the cell type
    using cell_type = T;

    /**
     * @brief Construct a hex grid with given size and default cell value.
     * @param size Pair(width, height) specifying number of columns and rows.
     * @param defaultValue Value to initialize each cell with (defaults to T()).
     */
    HexagonGrid(const std::pair<int, int>& size, const T& defaultValue = T());

    /// Get the number of columns in the grid
    virtual const int getWidth() const { return width_; }
    /// Get the number of rows in the grid
    virtual const int getHeight() const { return height_; }

    /**
     * @brief Retrieve a cell value by offset coordinates.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     * @return Copy of the cell value.
     * @throws std::out_of_range if (x,y) is outside grid bounds.
     */
    T get(int x, int y) const;

    /**
     * @brief Assign a new value to a cell by offset coordinates.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     * @param value New value to set.
     * @throws std::out_of_range if (x,y) is outside grid bounds.
     */
    void set(int x, int y, const T& value);

    /** @brief Set new width of map. */
    virtual void addWidth(int delta);
    
    /** @brief Set new height of map. */
    virtual void addHeight(int delta);

    // Iterators for range-based loops and STL compatibility
    auto begin()       { return grid_.begin(); }
    auto end()         { return grid_.end();   }
    auto begin()  const{ return grid_.cbegin(); }
    auto end()    const{ return grid_.cend();   }
    auto cbegin() const{ return grid_.cbegin(); }
    auto cend()   const{ return grid_.cend();   }

private:
    int width_;            ///< Number of columns
    int height_;           ///< Number of rows
    std::vector<T> grid_;  ///< Contiguous storage of cells
};

// Implementation of template methods
#include "Widgets/HexagonGrid.tpp"

#endif // HEXAGONGRID_HPP
