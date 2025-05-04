#ifndef PLAYABLEGROUND_HPP
#define PLAYABLEGROUND_HPP

#include "SDL.h"
#include "BlitTarget.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Player.hpp"
#include "Cells/Grounds/Ground.hpp"
#include "GameElements/GameElement.hpp"
#include "Displayers/FenceDisplayer.hpp"
#include <unordered_set>
#include <queue>
#include <memory>

class PlayableGround: public Ground, public std::enable_shared_from_this<PlayableGround> {
public:
    static std::shared_ptr<PlayableGround> cast(const std::weak_ptr<Cell>& obj);
    static bool is(const std::weak_ptr<Cell>& obj);
    static void init();
    static void quit();

    PlayableGround(const Point& pos, const std::shared_ptr<Player>& owner);
    PlayableGround(const Point& pos);

    std::shared_ptr<Player> getOwner();
    std::shared_ptr<Player> getOldOwner();
    void setOwner(std::shared_ptr<Player> owner);

    void display(const std::weak_ptr<BlitTarget>& target) override;

    bool hasFences() const;
    void displayFences(const std::weak_ptr<Texture>& target);
    void displayElement(const std::weak_ptr<Texture>& target);
    void displayShield(const std::weak_ptr<Texture>& target);
    void displayCross(const std::weak_ptr<Texture>& target);
    void displaySelectable(const std::weak_ptr<Texture>& target, const bool& selected = false);

    bool isLinked();
    void updateLinked();
    void link(const std::weak_ptr<Player>& owner);
    
    void freeTroops();
    std::vector<std::weak_ptr<Town>> getTowns();
    std::shared_ptr<Town> getNearestTown();
    void updateIncome();

    std::shared_ptr<GameElement> getElement();
    void setElement(const std::shared_ptr<GameElement>& elt);

    int getShield() const;
    void updateSelectable(int strength);
    bool isSelectable() const { return selectable_; }

private:
    static FenceDisplayer fenceDisplayer_;
    static std::vector<std::shared_ptr<Texture>> shieldSprites_;
    static std::shared_ptr<Texture> smallSelectableSprite_;
    static std::shared_ptr<Texture> selectableSprite_;
    static std::shared_ptr<Texture> crossSprite_;

    std::shared_ptr<Player> oldOwner_;
    std::shared_ptr<Player> owner_;
    HexagonDisplayer plate_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
    HexagonDisplayer lostPlate_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
    std::shared_ptr<GameElement> element = nullptr;
    bool hasPlate_ = false;
    bool selectable_ = false;

    bool isLinked(std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void unlink(std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void link(const std::weak_ptr<Player>& owner, std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void updateSelectable(int strength, std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void getTowns(std::queue<std::weak_ptr<PlayableGround>>& toVisit, std::unordered_set<std::shared_ptr<PlayableGround>>& visited, std::vector<std::weak_ptr<Town>>& towns);
    std::shared_ptr<Town> getNearestTown(std::queue<std::weak_ptr<PlayableGround>>& toVisit, std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void freeTroops(std::unordered_set<std::shared_ptr<PlayableGround>>& visited);

    void setSelectable(bool selectable);
};

#endif
