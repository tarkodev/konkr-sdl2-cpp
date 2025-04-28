#ifndef UNIT_BANDIT_HPP
#define UNIT_BANDIT_HPP

#include "logic/Troop.hpp"

class Bandit : public Troop {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Bandit() = default;

    /* Displayer */
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

private:
    static Texture* sprite_;
};

#endif
