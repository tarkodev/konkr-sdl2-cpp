#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

//! trier du plus bas au plus haut niveau (SDL est l'intermédiaire)
#include "SDL.h"
#include "HexagonGrid.hpp"
#include "Texture.hpp"
#include "Cell.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Player.hpp"
#include "logic/Troop.hpp"
#include "logic/units/Castle.hpp"
#include "logic/units/Camp.hpp"
#include "Displayer.hpp"
#include "BlitTarget.hpp"
#include "Font.hpp"
#include "TreasuryDisplayer.hpp"

#include <memory>
#include <utility>
#include <optional>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>

/**
 * @brief Classe représentant une carte de jeu.
 *
 * Hérite de HexagonGrid<int> pour gérer la logique de la grille d'hexagons,
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, etc.).
 */
class GameMap : public HexagonGrid<std::shared_ptr<Cell>>, public Displayer {
public:
    static void init();
    static std::shared_ptr<Cell> createCell(char letter, Point pos);
    static std::shared_ptr<GameElement> createGameElement(char letter, Point pos);
    
    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize);
    GameMap(const Point& pos, const Size size, const std::string mapFile);

    void setProportionalSize(const Size size);
    void selectCell(const Point& pos);

    void setPos(const Point& pos) override;

    void refresh();

    int getMaxTreasuryOfCurrentPlayer();
    void buyTroop(const std::shared_ptr<GameElement>& elt);

    bool hasTroopSelected();

    void handleEvent(SDL_Event &event);
    void display(const std::weak_ptr<BlitTarget>& target) override;

    void nextPlayer();

    int getWidth() const override;
    int getHeight() const override;

private:
    static std::mt19937 gen_;

    double ratio_ = 0;

    std::weak_ptr<PlayableGround> selectedCell_;
    std::weak_ptr<Town> townToShowTreasury_;
    std::weak_ptr<Camp> campToShowTreasury_;
    bool hasSelection_ = false;

    std::vector<std::weak_ptr<Player>> players_;
    std::weak_ptr<Player> currentPlayer_;

    std::vector<std::weak_ptr<Troop>> movedTroops_;
    std::shared_ptr<GameElement> boughtElt_;
    std::shared_ptr<Troop> selectedTroop_;
    std::weak_ptr<PlayableGround> selectedTroopCell_;
    std::shared_ptr<PlayableGround> selectedNewTroopCell_;
    std::unordered_set<std::shared_ptr<PlayableGround>> potentialTownCells_;
    int selectedPlayerNum_ = 0;

    Size calcSize_;
    std::shared_ptr<Texture> calc_ = nullptr;

    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize, const std::string mapFile);
    void loadMap(const std::string& mapFile);
    std::pair<int,int> getSizeOfMapFile(const std::string& mapFile);

    void updateNeighbors();
    void createCalcs();
    void updateSelectedCell();
    bool isSelectableTroop(const std::weak_ptr<PlayableGround>& pg);
    void updateCursor();
    void searchNextPlayer();
    void updateLinks();
    void initGame();
    void defrayBandits(const std::weak_ptr<Player>& player);
    void checkDeficits(std::weak_ptr<Player>& player);
    void startTurn(std::weak_ptr<Player>& player);
    bool isMovedTroop(const std::weak_ptr<Troop>& troop);

    void updateLostElements();
    void updateFreeTroops(const std::weak_ptr<Player>& player);
    std::unordered_map<std::shared_ptr<PlayableGround>, int> getTreasuresOfCurrentPlayers();
    bool placeCastle(const std::weak_ptr<Castle>& castle, const std::weak_ptr<PlayableGround>& to);
    void moveTroop(const std::weak_ptr<PlayableGround>& from, const std::weak_ptr<PlayableGround>& to);
    void moveBandit(const std::weak_ptr<PlayableGround>& from, const std::weak_ptr<PlayableGround>& to);
    void moveBandits();
    void updateIncomes(std::weak_ptr<Player>& player);
    void updateIncomes(std::shared_ptr<Player>& player);
    void updateMovables();

    void onMouseButtonDown(SDL_Event& event);
    void onMouseMotion(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    void refreshIslands();
    void refreshPlates();
    void refreshSelectables();
    void refreshFences();
    void refreshElements();
};

#endif
