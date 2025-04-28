#ifndef UNIT_HERO_HPP
#define UNIT_HERO_HPP

#include "logic/Troop.hpp"

class Hero : public Troop {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Hero() = default;

    /* Displayer */
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 4;
    static constexpr int COST     = 80;
    static constexpr int UPKEEP   = 54;

private:
    static Texture* sprite_;
};

#endif
