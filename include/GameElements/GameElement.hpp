#ifndef LOGIC_GAMEELEMENT_HPP
#define LOGIC_GAMEELEMENT_HPP

#include <string>
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "Displayers/Displayer.hpp"

class Player;

/**
 * @brief Élément logique posé sur une Cell (unité, bâtiment, nuisible…).
 */
class GameElement : public Displayer {
public:
    virtual ~GameElement() = default;

    /* --- Accesseurs génériques --- */
    virtual const int getStrength() const = 0;
    virtual const int getCost()     const = 0;
    virtual const int getUpkeep()   const = 0;

    /* --- Displayer --- */
    virtual void display(const std::weak_ptr<BlitTarget>& target) const override = 0;

    virtual const bool isLost() const { return lost_; };
    virtual void lost() { lost_ = true; };

protected:
    GameElement(const Point& pos, const Size& size);

    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;

    bool lost_ = false;
};

#endif
