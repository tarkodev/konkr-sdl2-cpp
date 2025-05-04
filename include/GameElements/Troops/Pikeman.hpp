#ifndef UNIT_PIKEMAN_HPP
#define UNIT_PIKEMAN_HPP

#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "GameElements/Troops/Troop.hpp"

class Pikeman : public Troop {
public:
    static std::shared_ptr<Pikeman> cast(const std::weak_ptr<GameElement>& obj);
    static const bool is(const std::weak_ptr<GameElement>& obj);
    
    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Pikeman(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /* --- Accesseurs génériques --- */
    const int getStrength() const override { return STRENGTH; };
    const int getCost()     const override { return COST; };
    const int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 2;
    static constexpr int COST     = 20;
    static constexpr int UPKEEP   = 6;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
