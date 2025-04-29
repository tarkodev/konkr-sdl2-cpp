#ifndef PLAYERTERRITORY_HPP
#define PLAYERTERRITORY_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "Player.hpp"
#include "Ground.hpp"
#include "logic/GameElement.hpp"
#include "FenceDisplayer.hpp"
#include <unordered_set>

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

    bool hasFences() const;
    void displayFences(const Texture* target, const Point& pos);
    void displayElement(const Texture* target, const Point& pos);
    void displayShield(const Texture* target, const Point& pos);
    void displaySelectable(const Texture* target, const Point& pos);

    bool isLinked();
    void updateLinked();

    GameElement* getElement();
    void setElement(GameElement* elt);

    int getShield() const;
    void updateSelectable(int strength);
    bool isSelectable() const { return selectable_; }

private:
    static FenceDisplayer fenceDisplayer_;
    static std::vector<Texture*> shieldSprites_;
    static Texture* selectableSprite_;

    Player* oldOwner_ = nullptr;
    Player* owner_ = nullptr;
    HexagonDisplayer plate_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    HexagonDisplayer lostPlate_ = HexagonDisplayer{nullptr, -1, nullptr, nullptr, nullptr, nullptr, nullptr};
    GameElement* element = nullptr;
    bool hasPlate_ = false;
    bool selectable_ = false;

    bool isLinked(std::unordered_set<PlayableGround*>& visited);
    void unlink(std::unordered_set<PlayableGround*>& visited);
    void updateSelectable(int strength, std::unordered_set<PlayableGround*>& visited);

    void setSelectable(bool selectable);
};

#endif
