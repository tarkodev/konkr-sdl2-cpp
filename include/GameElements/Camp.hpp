#ifndef UNIT_CAMP_HPP
#define UNIT_CAMP_HPP

#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "GameElements/GameElement.hpp"
#include "Displayers/TreasuryDisplayer.hpp"

class Camp : public GameElement {
public:
    static std::shared_ptr<Camp> cast(const std::weak_ptr<GameElement>& obj);
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Camp(const Point& pos, const int& treasury = 0);

    /* Displayer */
    void setPos(const Point& pos) override;
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /* --- Accesseurs génériques --- */
    const int getStrength() const override { return STRENGTH; };
    const int getCost()     const override { return COST; };
    const int getUpkeep()   const override { return UPKEEP; };

    void addCoins(int coins);
    const int getTreasury() const;
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
