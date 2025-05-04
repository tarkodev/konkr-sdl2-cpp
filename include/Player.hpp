#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Displayers/HexagonDisplayer.hpp"
#include "Utils/ColorUtils.hpp"
#include "GameElements/Town.hpp"
#include <unordered_set>

class PlayableGround;

using namespace ColorUtils;

class Player {
public:
    static void init(const std::shared_ptr<SDL_Renderer>& renderer);
    static void quit();

    Player(const GroundColor& color);
    ~Player();

    HexagonDisplayer& getPlate();
    HexagonDisplayer& getLostPlate();

    void updateTowns();
    bool hasSelected() const { return selected_; };
    bool hasTowns();
    std::vector<std::weak_ptr<PlayableGround>> getTownCells();

    void addTownCell(std::weak_ptr<PlayableGround> town);

    void onTurnStart();
    void onTurnEnd();

private:
    static std::weak_ptr<SDL_Renderer> renderer_;
    static HexagonDisplayer plateDisplayer_;
    
    GroundColor color_;

    HexagonDisplayer plate_;
    HexagonDisplayer lostPlate_;
    std::vector<std::weak_ptr<PlayableGround>> townCells_;

    bool selected_ = false;
};

#endif
