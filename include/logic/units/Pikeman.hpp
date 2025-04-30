#ifndef UNIT_PIKEMAN_HPP
#define UNIT_PIKEMAN_HPP

#include "BlitTarget.hpp"
#include "logic/Troop.hpp"

class Pikeman : public Troop {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Pikeman(const Point& pos);

    /* Displayer */
    void display(const BlitTarget* target) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 2;
    static constexpr int COST     = 20;
    static constexpr int UPKEEP   = 6;

private:
    static Texture* sprite_;
};

#endif
