#ifndef UNIT_CASTLE_HPP
#define UNIT_CASTLE_HPP

#include "BlitTarget.hpp"
#include "GameElements/GameElement.hpp"

class Castle : public GameElement {
public:
    /* Chargement des ressources partagées */
    static void init();
    static void quit();
    static std::shared_ptr<Castle> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);

    explicit Castle(const Point& pos);

    /* Displayer */
    void display(const std::weak_ptr<BlitTarget>& target) override;

    /* --- Accesseurs génériques --- */
    int getStrength() const override { return STRENGTH; };
    int getCost()     const override { return COST; };
    int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 2;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;

private:
    static std::shared_ptr<Texture> sprite_;
};

#endif
