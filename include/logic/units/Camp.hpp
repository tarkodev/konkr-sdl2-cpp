#ifndef UNIT_CAMP_HPP
#define UNIT_CAMP_HPP

#include "BlitTarget.hpp"
#include "logic/GameElement.hpp"
#include "TreasuryDisplayer.hpp"

class Camp : public GameElement {
public:
    static std::shared_ptr<Camp> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);

    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Camp(const Point& pos);

    /* Displayer */
    void setPos(const Point& pos) override;
    void display(const std::weak_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

    void addCoins(int coins);
    int getTreasury() const;
    void displayTreasury(const std::weak_ptr<BlitTarget>& target);

protected:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

private:
    static std::shared_ptr<Texture> sprite_;

    TreasuryDisplayer treasuryDisplayer_;
    int treasury_ = 0;
};

#endif
