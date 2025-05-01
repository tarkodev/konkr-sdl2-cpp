#ifndef LOGIC_PLAYERELEMENT_HPP
#define LOGIC_PLAYERELEMENT_HPP

#include "SDL.h"
#include "logic/GameElement.hpp"
#include "Texture.hpp"

class Troop: public GameElement {
public:
    /** Charge le sprite commun à la classe (à appeler une seule fois). */
    static void init();

    virtual ~Troop() = default;

    bool isFree() const;
    void setFree(bool free);

protected:
    static Texture* shadow_;
    static Texture* lostSprite_;

    bool free_ = false;

    Troop(const Point& pos);

    void displaySprite(const BlitTarget* target, const Texture* sprite);
};

#endif
