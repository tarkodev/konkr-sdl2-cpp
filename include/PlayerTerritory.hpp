#ifndef PLAYERTERRITORY_HPP
#define PLAYERTERRITORY_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "Player.hpp"
#include "Territory.hpp"

class PlayerTerritory: public Territory {
public:
    static const std::string TYPE;

    PlayerTerritory(Player *owner);

    Player* getOwner();
    void setOwner(Player *owner);

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;

    static void init();

private:
    HexagonDisplayer displayer_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    Player* owner_ = nullptr;
};

#endif
