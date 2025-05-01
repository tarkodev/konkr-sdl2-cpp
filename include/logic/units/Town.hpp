#ifndef TOWN_HPP
#define TOWN_HPP

#include "BlitTarget.hpp"
#include "TreasuryDisplayer.hpp"
#include "logic/GameElement.hpp"

class Town : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Town(const Point& pos);

    /* Displayer */
    void display(const BlitTarget* target) override;
    const Size getSize() const override;

    void displayTreasury(const BlitTarget* target);

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
    static Texture* sprite_;
    static Texture* selectSprite_;

    std::unique_ptr<TreasuryDisplayer> treasuryDisplayer_;
    bool selected_ = false;
    int treasury_ = 0;
    int income_ = 0;
};

#endif
