#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "Texture.hpp"
#include "SDL2/SDL_image.h"
#include "HexagonDisplayer.hpp"
#include "ColorUtils.hpp"

using namespace ColorUtils;

class Player {
public:
    static void init(SDL_Renderer *renderer);

    Player(const std::string& name_, const GroundColor& color);
    ~Player();

    HexagonDisplayer& getPlate();
    HexagonDisplayer& getLostPlate();

private:
    static SDL_Renderer *renderer_;
    static HexagonDisplayer plateDisplayer;
    
    std::string name_;
    GroundColor color_;

    HexagonDisplayer plate_;
    HexagonDisplayer lostPlate_;
};

#endif
