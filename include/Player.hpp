#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "Texture.hpp"
#include "SDL2/SDL_image.h"

class Player {
public:
    Player(SDL_Color color);
    ~Player();

    Texture* getHexagonSprite() const;

    static void init(SDL_Renderer* renderer, Texture* plate);

private:
    static SDL_Renderer *renderer_;
    static Texture* sprite_;

    std::string name = "Joueur"; //! temp
    SDL_Color color_;
    Texture* plate_;
};

#endif
