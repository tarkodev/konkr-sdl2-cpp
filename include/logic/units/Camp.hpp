#ifndef UNIT_CAMP_HPP
#define UNIT_CAMP_HPP

#include "BlitTarget.hpp"
#include "logic/GameElement.hpp"

class Camp : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Camp(const Point& pos);

    /* Displayer */
    void display(const BlitTarget* target) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

    void addCoins(int coins);

protected:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

private:
    static Texture* sprite_;
    int treasury_ = 0;
};

#endif
