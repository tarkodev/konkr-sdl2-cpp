#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "Texture.hpp"
#include "SDL2/SDL_image.h"
#include "HexagonDisplayer.hpp"

class Player {
public:
    Player(const std::string& name_, const SDL_Color& color);
    ~Player();

    SDL_Color& getColor();

    HexagonDisplayer& getPlateDisplayer();
    void setPlateDisplayer(HexagonDisplayer& plateDisplayer);
    bool hasPlate() const;

private:
    HexagonDisplayer plateDisplayer_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    std::string name_;
    SDL_Color color_;
    bool hasPlate_ = false;
};

#endif
