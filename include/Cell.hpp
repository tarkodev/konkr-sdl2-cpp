#ifndef CELL_HPP
#define CELL_HPP

#include <SDL.h>
#include "Texture.hpp"
#include "Player.hpp"

class Cell {
public:
    virtual void handleEvents(SDL_Event &event) = 0;
    virtual void draw(Point& pos) = 0;

    static void init(SDL_Renderer *renderer, Texture* plate);
    static Texture* getSprite();

private:
    static SDL_Renderer* renderer_;
    static Texture* sprite_;
};

#endif
