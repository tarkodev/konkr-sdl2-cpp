// logic/Building.hpp
#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "GameElement.hpp"

/**
 * @brief Bâtiment statique (Town, Castle, Camp…).
 */
class Building : public GameElement {
public:
    using GameElement::GameElement;
    ~Building() override = default;

    /* Les bâtiments ne bougent pas */
    void display(const Texture* target, const Point& pos) override = 0;
};

#endif
