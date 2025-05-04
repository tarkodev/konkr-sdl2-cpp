#ifndef UNIT_HERO_HPP
#define UNIT_HERO_HPP

#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "GameElements/Troops/Troop.hpp"

class Hero : public Troop {
public:
    static std::shared_ptr<Hero> cast(const std::weak_ptr<GameElement>& obj);
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Hero(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /* --- Accesseurs génériques --- */
    const int getStrength() const override { return STRENGTH; };
    const int getCost()     const override { return COST; };
    const int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 4;
    static constexpr int COST     = 80;
    static constexpr int UPKEEP   = 54;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
