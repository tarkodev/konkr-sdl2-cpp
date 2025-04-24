// logic/Movable.hpp
#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include "Point.hpp"
#include <optional>

/**
 * @brief Interface d’un élément pouvant se déplacer sur la carte.
 */
class Movable {
public:
    virtual ~Movable() = default;

    /**
     * @return la position actuelle en coordonnées (col,row) offset.
     */
    virtual Point getPosition() const = 0;

    /**
     * @brief Tentative de déplacement.  Retourne true si succès.
     *
     * L’implémentation concrète vérifiera la validité (coût, force, etc.).
     */
    virtual bool moveTo(const Point& dest) = 0;
};

#endif
