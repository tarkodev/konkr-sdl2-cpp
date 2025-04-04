#ifndef CELL_HPP
#define CELL_HPP

#include <SDL.h>
#include "Player.hpp"

class Cell {
public:
    Cell(SDL_Renderer *renderer, Player* owner);
    
    void setOwner(Player* owner);

    void handleEvents();
    void draw(SDL_Point& pos);

    static void setDefaultSprite(SDL_Texture* defaultSprite, SDL_Rect& size) {
        defaultSprite_ = defaultSprite;
        spriteSize_ = size;
    }

private:
    static SDL_Texture* defaultSprite_;
    static SDL_Rect spriteSize_;

    SDL_Renderer *renderer_;

    Player *owner_ = nullptr;
    SDL_Texture* sprite_ = nullptr;
    SDL_Rect size_;
};

#endif
