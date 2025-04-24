#ifndef TROOP_HPP
#define TROOP_HPP

#include "GameElement.hpp"
#include "Movable.hpp"

/**
 * @brief Unité mobile (Villager, Pikeman, Knight, Hero, Bandit).
 */
class Troop : public GameElement, public Movable {
public:
    Troop(int strength,
           int cost,
           int upkeep,
           Player* owner = nullptr);

    int  getUpkeep() const { return upkeep_; }

    /* --- Implémentation Movable minimale --- */
    Point getPosition() const override { return position_; }
    bool  moveTo(const Point& dest) override;   ///< déplacement “brut” pour l’instant

protected:
    int upkeep_;
    Point position_{-1,-1};   ///< position courante
};

#endif
