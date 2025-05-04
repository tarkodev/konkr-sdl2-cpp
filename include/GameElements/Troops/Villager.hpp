#ifndef UNIT_VILLAGER_HPP
#define UNIT_VILLAGER_HPP

#include "BlitTarget.hpp"
#include "GameElements/Troops/Troop.hpp"

class Villager : public Troop {
public:
    static std::shared_ptr<Villager> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);
    
    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Villager(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 10;
    static constexpr int UPKEEP   = 2;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
