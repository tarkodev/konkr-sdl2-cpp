#ifndef PLAYERTERRITORY_HPP
#define PLAYERTERRITORY_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "Player.hpp"
#include "Ground.hpp"
#include "logic/GameElement.hpp"
#include "PlayableGround.hpp"
#include "FenceDisplayer.hpp"

class PlayableGround: public Ground {
public:
    static const std::string TYPE;
    static void init();

    PlayableGround(Player *owner);
    PlayableGround();

    Player* getOwner();
    Player* getOldOwner();
    void setOwner(Player *owner);

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;
    void displayFences(const Texture* target, const Point& pos);

    GameElement* getElement();
    void setElement(GameElement* elt);

private:
    static FenceDisplayer fenceDisplayer_;

    Player* oldOwner_ = nullptr;
    Player* owner_ = nullptr;
    HexagonDisplayer plate_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    HexagonDisplayer lostPlate_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    GameElement* element = nullptr;
    bool hasPlate_ = false;
};

#endif
