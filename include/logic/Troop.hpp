#ifndef LOGIC_TROOP_HPP
#define LOGIC_TROOP_HPP

#include "logic/GameElement.hpp"
#include "logic/Movable.hpp"

/**
 * @brief Unité mobile contrôlée (ou non) par le joueur.
 */
class Troop : public GameElement, public Movable {
public:
    Troop(int strength, int cost, int upkeep, Player* owner = nullptr);

    /* --- Movable --- */
    Point getPosition() const override { return position_; }
    bool  moveTo(const Point& dest) override;

    /** Charge le sprite commun à la classe (à appeler une seule fois). */
    static void init();

protected:
    static Texture* shadow;

    Point position_{-1, -1};   ///< position (col,row) offset
};

#endif
