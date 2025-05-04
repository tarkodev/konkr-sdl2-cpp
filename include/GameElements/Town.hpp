#ifndef UNIT_TOWN_HPP
#define UNIT_TOWN_HPP

#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "Displayers/TreasuryDisplayer.hpp"
#include "GameElements/GameElement.hpp"

class Town : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();
    static void quit();
    static std::shared_ptr<Town> cast(const std::weak_ptr<GameElement>& obj);
    static const bool is(const std::weak_ptr<GameElement>& obj);

    explicit Town(const Point& pos, const int& treasury = 0);

    /* Displayer */
    void setPos(const Point& pos) override;
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    void displayTreasury(const std::weak_ptr<BlitTarget>& target);

    /* --- Accesseurs génériques --- */
    const int getStrength() const override;
    const int getCost()     const override;
    const int getUpkeep()   const override;

    const int getTreasury() const;
    void setTreasury(int treasury);
    void updateTreasury();

    const int getIncome() const;
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
