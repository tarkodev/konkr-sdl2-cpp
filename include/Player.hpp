#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "Texture.hpp"
#include "SDL2/SDL_image.h"
#include "HexagonDisplayer.hpp"

class Player {
public:
    static void init(SDL_Renderer *renderer);

    Player(const std::string& name_, const SDL_Color& color);
    ~Player();

    SDL_Color& getColor();

    HexagonDisplayer& getPlate();

private:
    static SDL_Renderer *renderer_;
    static HexagonDisplayer plateDisplayer;
    
    std::string name_;
    SDL_Color color_;

    HexagonDisplayer plate_;
};

#endif
