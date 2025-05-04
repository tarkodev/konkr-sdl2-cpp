#ifndef UNIT_PIKEMAN_HPP
#define UNIT_PIKEMAN_HPP

#include "BlitTarget.hpp"
#include "GameElements/Troops/Troop.hpp"

class Pikeman : public Troop {
public:
    static std::shared_ptr<Pikeman> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);
    
    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Pikeman(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 2;
    static constexpr int COST     = 20;
    static constexpr int UPKEEP   = 6;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
