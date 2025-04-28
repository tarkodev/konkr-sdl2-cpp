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
    virtual ~GameElement() = default;

    /* --- Accesseurs génériques --- */
    int      getStrength()   const { return strength_; }
    int      getCost()       const { return cost_;     }   // 0 si gratuit
    int      getUpkeep()     const { return upkeep_;   }   // 0 si aucun entretien
    Player*  getOwner()      const { return owner_;    }
    void     setOwner(Player* p)   { owner_ = p;       } //! supprimer (camp n'a pas de propriétaire, créer classe abstraite PlayerTroop qui n'inclut pas bandit et PlayerBuilding qui n'inclut pas de camp)

    /* --- Cycle de tour --- */
    virtual void onTurnStart()  {}   ///< début du tour du propriétaire
    virtual void onTurnEnd()    {}   ///< fin du tour du propriétaire
    virtual void onDestroyed()  {}   ///< quand l’élément est détruit

    /* --- Displayer --- */
    /** Affiche le sprite dans la texture @p target au centre de @p pos (coord. pixel). */
    virtual void display(const Texture* target, const Point& pos) override = 0;
    /** Taille (pixels) du sprite. */
    virtual const Size getSize() const override = 0;

    /** Charge le sprite commun à la classe (à appeler une seule fois). */
    static void init(SDL_Renderer* renderer);

protected:
    GameElement(int strength, int cost, int upkeep, Player* owner = nullptr);

    int strength_;
    int cost_;
    int upkeep_;
    Player* owner_;

    /* --- Ressources graphiques partagées --- */
    static SDL_Renderer* renderer;
};

#endif
