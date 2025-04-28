#ifndef LOGIC_GAMEELEMENT_HPP
#define LOGIC_GAMEELEMENT_HPP

#include <string>
#include "Displayer.hpp"

class Player;

/**
 * @brief Élément logique posé sur une Cell (unité, bâtiment, nuisible…).
 */
class GameElement : public Displayer {
public:
    /** Charge le sprite commun à la classe (à appeler une seule fois). */
    static void init(SDL_Renderer* renderer);

    virtual ~GameElement() = default;

    /* --- Accesseurs génériques --- */
    virtual int getStrength() const = 0;
    virtual int getCost()     const = 0;
    virtual int getUpkeep()   const = 0;

    /* --- Displayer --- */
    virtual void display(const Texture* target, const Point& pos) override = 0;
    virtual const Size getSize() const override = 0;

protected:
    GameElement() = default;

    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;
    Player* owner_;

    /* --- Ressources graphiques partagées --- */
    static SDL_Renderer* renderer;
};

#endif
