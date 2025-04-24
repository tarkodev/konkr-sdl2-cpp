#ifndef SEA_HPP
#define SEA_HPP

#include "Cell.hpp"

class Sea: public Cell {
public:
    static const std::string TYPE;

    Sea();

    const std::string getType() override;
};

#endif
