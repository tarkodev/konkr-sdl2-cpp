#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "Texture.hpp"
#include "HexagonDisplayer.hpp"
#include "ColorUtils.hpp"
#include "logic/units/Town.hpp"
#include <unordered_set>

class PlayableGround;

using namespace ColorUtils;

class Player {
public:
    static void init(SDL_Renderer *renderer);

    Player(const std::string& name_, const GroundColor& color);
    ~Player();

    HexagonDisplayer& getPlate();
    HexagonDisplayer& getLostPlate();

    void updateTowns();
    bool hasSelected() const { return selected_; };
    bool hasTowns();
    std::unordered_set<PlayableGround*> getTownCells();

    void addTownCell(PlayableGround* town);

    void onTurnStart();
    void onTurnEnd();

private:
    static SDL_Renderer *renderer_;
    static HexagonDisplayer plateDisplayer;
    
    std::string name_;
    GroundColor color_;

    HexagonDisplayer plate_;
    HexagonDisplayer lostPlate_;
    std::unordered_set<PlayableGround*> townCells_;

    bool selected_ = false;
};

#endif
