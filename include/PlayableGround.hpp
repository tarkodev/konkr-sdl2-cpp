#ifndef PLAYABLEGROUND_HPP
#define PLAYABLEGROUND_HPP

#include "SDL.h"
#include "BlitTarget.hpp"
#include "Texture.hpp"
#include "Player.hpp"
#include "Ground.hpp"
#include "logic/GameElement.hpp"
#include "FenceDisplayer.hpp"
#include <unordered_set>
#include <queue>

class PlayableGround: public Ground {
public:
    static const std::string TYPE;
    static void init();

    PlayableGround(const Point& pos, Player *owner);
    PlayableGround(const Point& pos);

    Player* getOwner();
    Player* getOldOwner();
    void setOwner(Player *owner);

    const std::string getType() override;
    void display(const std::shared_ptr<BlitTarget>& target) override;

    bool hasFences() const;
    void displayFences(const std::shared_ptr<Texture>& target);
    void displayElement(const std::shared_ptr<Texture>& target);
    void displayShield(const std::shared_ptr<Texture>& target);
    void displaySelectable(const std::shared_ptr<Texture>& target);

    bool isLinked();
    void updateLinked();
    void link(Player* owner);
    
    void freeTroops();
    std::vector<Town*> getTowns();
    Town* getNearestTown();
    void updateIncome();

    GameElement* getElement();
    void setElement(GameElement* elt);

    int getShield() const;
    void updateSelectable(int strength);
    bool isSelectable() const { return selectable_; }

private:
    static FenceDisplayer fenceDisplayer_;
    static std::vector<std::shared_ptr<Texture>> shieldSprites_;
    static std::shared_ptr<Texture> selectableSprite_;

    Player* oldOwner_ = nullptr;
    Player* owner_ = nullptr;
    HexagonDisplayer plate_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
    HexagonDisplayer lostPlate_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
    GameElement* element = nullptr;
    bool hasPlate_ = false;
    bool selectable_ = false;

    bool isLinked(std::unordered_set<PlayableGround*>& visited);
    void unlink(std::unordered_set<PlayableGround*>& visited);
    void link(Player* owner, std::unordered_set<PlayableGround*>& visited);
    void updateSelectable(int strength, std::unordered_set<PlayableGround*>& visited);
    void getTowns(std::queue<PlayableGround*>& toVisit, std::unordered_set<PlayableGround*>& visited, std::vector<Town*>& towns);
    Town* getNearestTown(std::queue<PlayableGround*>& toVisit, std::unordered_set<PlayableGround*>& visited);
    void freeTroops(std::unordered_set<PlayableGround*>& visited);

    void setSelectable(bool selectable);
};

#endif
