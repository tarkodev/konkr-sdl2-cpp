#ifndef TERRITORY_HPP
#define TERRITORY_HPP

#include <SDL.h>
#include "Texture.hpp"
#include "Player.hpp"
#include "Cell.hpp"

class Territory: public Cell {
public:
    Territory();

    void handleEvents(SDL_Event &event) override;
    void draw(Point& pos) override;

    static void init(SDL_Renderer* renderer, Texture* plate);
    static Texture* getSprite();

private:
    static SDL_Renderer* renderer_;
    static Texture* sprite_;
};

#endif
