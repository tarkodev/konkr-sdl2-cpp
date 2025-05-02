#ifndef UNIT_TOWN_HPP
#define UNIT_TOWN_HPP

#include "BlitTarget.hpp"
#include "TreasuryDisplayer.hpp"
#include "logic/GameElement.hpp"

class Town : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Town(const Point& pos);

    /* Displayer */
    void display(const std::shared_ptr<BlitTarget>& target) override;

    void displayTreasury(const std::shared_ptr<BlitTarget>& target);

    /* --- Accesseurs génériques --- */
    int getStrength() const override;
    int getCost()     const override;
    int getUpkeep()   const override;

    int getTreasury() const;
    void setTreasury(int treasury);
    void updateTreasury();

    int getIncome() const;
    void setIncome(int income);
    void addIncome(int coins);

    void setSelected(bool selected);

protected:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;

private:
    static std::shared_ptr<Texture> sprite_;
    static std::shared_ptr<Texture> selectSprite_;

    TreasuryDisplayer treasuryDisplayer_;
    bool selected_ = false;
    int treasury_ = 0;
    int income_ = 0;
};

#endif
