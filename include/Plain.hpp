#ifndef PLAIN_HPP
#define PLAIN_HPP

#include <SDL.h>
#include "Texture.hpp"
#include "Player.hpp"
#include "Territory.hpp"

class Plain: public Territory {
public:
    Plain();

    void handleEvents(SDL_Event &event) override;
    void draw(Texture* texture, const Point& pos) override;

    static void init();

protected:
    static Texture* sprite_;
};

#endif
