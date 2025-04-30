#ifndef UNIT_VILLAGER_HPP
#define UNIT_VILLAGER_HPP

#include "BlitTarget.hpp"
#include "logic/Troop.hpp"

class Villager : public Troop {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Villager(const Point& pos);

    /* Displayer */
    void display(const BlitTarget* target) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 10;
    static constexpr int UPKEEP   = 2;

private:
    static Texture* sprite_;
};

#endif
