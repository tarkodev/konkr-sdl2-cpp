#ifndef UNIT_HERO_HPP
#define UNIT_HERO_HPP

#include "BlitTarget.hpp"
#include "GameElements/Troops/Troop.hpp"

class Hero : public Troop {
public:
    static std::shared_ptr<Hero> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);

    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Hero(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 4;
    static constexpr int COST     = 80;
    static constexpr int UPKEEP   = 54;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
