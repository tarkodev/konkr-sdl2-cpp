#ifndef PLAYERTERRITORY_HPP
#define PLAYERTERRITORY_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "Player.hpp"
#include "Ground.hpp"
#include "logic/GameElement.hpp"
#include "PlayableGround.hpp"

class PlayableGround: public Ground {
public:
    static const std::string TYPE;

    PlayableGround(Player *owner);
    PlayableGround();

    Player* getOwner();
    void setOwner(Player *owner);

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;
    GameElement* getElement();
    void setElement(GameElement* elt);

private:
    Player* owner_ = nullptr;
    HexagonDisplayer plate_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    GameElement* element = nullptr;
    bool hasPlate_ = false;
};

#endif
