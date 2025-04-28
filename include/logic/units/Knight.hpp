#ifndef UNIT_KNIGHT_HPP
#define UNIT_KNIGHT_HPP

#include "logic/Troop.hpp"

class Knight : public Troop {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Knight() = default;

    /* Displayer */
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 3;
    static constexpr int COST     = 40;
    static constexpr int UPKEEP   = 18;

private:
    static Texture* sprite_;
};

#endif
