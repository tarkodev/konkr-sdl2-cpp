#ifndef UNIT_KNIGHT_HPP
#define UNIT_KNIGHT_HPP

#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "GameElements/Troops/Troop.hpp"

class Knight : public Troop {
public:
    static std::shared_ptr<Knight> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);

    /* Chargement des ressources partagées */
    static void init();
    static void quit();

    explicit Knight(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 3;
    static constexpr int COST     = 40;
    static constexpr int UPKEEP   = 18;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
