#ifndef WATER_HPP
#define WATER_HPP

#include "Cell.hpp"

class Water: public Cell {
public:
    static const std::string TYPE;

    Water();

    const std::string getType() override;
};

#endif
