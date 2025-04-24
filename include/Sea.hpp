#ifndef SEA_HPP
#define SEA_HPP

#include <SDL.h>
#include "Texture.hpp"
#include "Player.hpp"
#include "Cell.hpp"

class Sea: public Cell {
public:
    Sea();

    void handleEvents(SDL_Event &event) override;
    void draw(Texture* texture, const Point& pos) override;

    static void init(SDL_Renderer* renderer, Texture* plate);
    static Texture* getSprite();

private:
    static SDL_Renderer* renderer_;
    static Texture* sprite_;
};

#endif
