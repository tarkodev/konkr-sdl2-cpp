// logic/GameElement.hpp
#ifndef GAMEELEMENT_HPP
#define GAMEELEMENT_HPP

#include <memory>
#include <string>
#include "Cell.hpp"
#include "Player.hpp"

/**
 * @brief Élément logique posé sur une Cell : unité ou bâtiment.
 *
 * Toutes les entités partageant une Force, un coût, un propriétaire éventuel
 * héritent de GameElement.
 */
class GameElement {
public:
    explicit GameElement(int strength,
                         int cost,
                         Player* owner = nullptr);

    virtual ~GameElement() = default;

    /* --- Accesseurs génériques --- */
    int getStrength()  const { return strength_; }
    int getCost()      const { return cost_;     }
    Player* getOwner() const { return owner_;    }
    void    setOwner(Player* p) { owner_ = p; }

    /* --- Logique de tour --- */
    virtual void onTurnStart()  {}          ///< déclenché au début du tour du propriétaire
    virtual void onTurnEnd()    {}          ///< déclenché à la fin du tour du propriétaire
    virtual void onDestroyed()  {}          ///< appelé quand l’élément est détruit

    /* --- Rendu --- */
    virtual void display(const Texture* target,
                         const Point& pos) = 0;

protected:
    int strength_;
    int cost_;
    Player* owner_;
};

#endif
