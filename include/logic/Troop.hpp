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

protected:
    static Texture* shadow;

    Troop() = default;

private:
    static SDL_Renderer* renderer_;
};

#endif
