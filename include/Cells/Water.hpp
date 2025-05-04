#ifndef WATER_HPP
#define WATER_HPP

#include "Cells/Cell.hpp"

class Water: public Cell {
public:
    static std::shared_ptr<Water> cast(const std::weak_ptr<Cell>& obj);
    static bool is(const std::weak_ptr<Cell>& obj);

    Water() = default;
};

#endif
