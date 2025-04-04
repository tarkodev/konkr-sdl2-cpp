#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "SDL2/SDL_image.h"

class Player {
public:
    Player(SDL_Renderer *renderer, SDL_Color color);
    ~Player();

    SDL_Texture* getHexagonSprite() const;
    SDL_Rect getHexagonSize() const;

    static void setDefaultHexagonSprite(SDL_Texture* defaultHexagonSprite, SDL_Rect& hexagonSize) {
        defaultHexagonSprite_ = defaultHexagonSprite;
        hexagonSize_ = hexagonSize;
    }

private:
    static SDL_Texture* defaultHexagonSprite_;
    static SDL_Rect hexagonSize_;


    SDL_Renderer *renderer_;

    std::string name = "Joueur"; //! temp
    SDL_Color color_;
    SDL_Texture* hexagonSprite_;
};

#endif
