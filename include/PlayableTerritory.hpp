#ifndef PLAYABLETERRITORY_HPP
#define PLAYABLETERRITORY_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "HexagonDisplayer.hpp"
#include "Player.hpp"
#include "Territory.hpp"

class PlayableTerritory: public Territory {
public:
    static const std::string TYPE;
    static void init();

    const std::string getType() override;

protected:
    PlayableTerritory() = default;
    
    static HexagonDisplayer plateDisplayer;
};

#endif
