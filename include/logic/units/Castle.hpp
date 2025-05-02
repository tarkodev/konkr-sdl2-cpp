#ifndef UNIT_CASTLE_HPP
#define UNIT_CASTLE_HPP

#include "BlitTarget.hpp"
#include "logic/GameElement.hpp"

class Castle : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Castle(const Point& pos);

    /* Displayer */
    void display(const std::shared_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 2;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
