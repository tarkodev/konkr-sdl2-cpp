#ifndef UNIT_BANDIT_HPP
#define UNIT_BANDIT_HPP

#include "BlitTarget.hpp"
#include "logic/Troop.hpp"

class Bandit : public Troop {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Bandit(const Point& pos);

    /* Displayer */
    void display(const std::shared_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
