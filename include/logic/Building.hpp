#ifndef LOGIC_BUILDING_HPP
#define LOGIC_BUILDING_HPP

#include "logic/GameElement.hpp"

/**
 * @brief Bâtiment statique (Town, Castle, Camp…).
 */
class Building : public GameElement {
public:
    using GameElement::GameElement;
    ~Building() override = default;

    /* Les bâtiments ne bougent pas → ne dérivent pas de Movable. */
};

#endif
