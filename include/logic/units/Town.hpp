#ifndef TOWN_HPP
#define TOWN_HPP

#include "BlitTarget.hpp"
#include "logic/GameElement.hpp"

class Town : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();

    explicit Town(const Point& pos);

    /* Displayer */
    void display(const BlitTarget* target) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

    int getCapital() const { return coins_; }
    void setCapital(int coins) { coins_ = coins; }
    void updateCapital() { coins_ = nextCoins_; };
    void resetNextCapital() { nextCoins_ = coins_; };

    int getNextCapital() const { return nextCoins_; }
    void setNextCapital(int coins) { nextCoins_ = coins; }
    int addNextCoins(int coins);

    void setSelected(bool selected) { selected_ = selected; }

protected:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

private:
    static Texture* sprite_;
    static Texture* selectSprite_;

    bool selected_ = false;
    int coins_ = 0;
    int nextCoins_ = 0;
};

#endif
