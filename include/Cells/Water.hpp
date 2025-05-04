#ifndef WATER_HPP
#define WATER_HPP

//------------------------------
// Project Core
//------------------------------
#include "Cells/Cell.hpp"  // Base class for all cell types

/**
 * @brief A terrain cell representing water.
 *
 * Water cells are non-playable obstacles (e.g., lakes, rivers)
 * that typically block movement and have no display logic of their own.
 */
class Water : public Cell {
public:
    /**
     * @brief Safely cast a generic Cell to a Water.
     * @param obj Weak pointer to the cell.
     * @return Shared pointer to Water or nullptr if the cast fails.
     */
    static std::shared_ptr<Water> cast(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Check if a generic Cell is actually a Water.
     * @param obj Weak pointer to the cell.
     * @return true if obj wraps a Water instance.
     */
    static const bool is(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Default constructor for a Water cell.
     * No special initialization required.
     */
    Water() = default;
};

#endif // WATER_HPP
