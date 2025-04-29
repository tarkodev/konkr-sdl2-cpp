#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "SDL.h"
#include <memory>
#include <utility>
#include <optional>
#include <vector>
#include "HexagonGrid.hpp"
#include "Texture.hpp"
#include "Cell.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Player.hpp"
#include "logic/Troop.hpp"

/**
 * @brief Classe représentant une carte de jeu.
 *
 * Hérite de HexagonGrid<int> pour gérer la logique de la grille d'hexagons,
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, etc.).
 */
class GameMap : public HexagonGrid<Cell*> {
public:
    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize);
    GameMap(const Point& pos, const Size size, const std::string mapFile);

    Size getSize() const;
    void setProportionalSize(const Size size);
    void selectCell(const Point& pos);

    void setPos(const Point& pos);
    Point getPos() const;

    void refreshElements();
    void refreshSelectables();
    void refresh();

    bool hasTroopSelected() {return selectedTroopCell_ != nullptr;};

    void handleEvent(SDL_Event &event);
    void draw();

    void endTurn();
    void test();

    static void init(SDL_Renderer* renderer);

private:
    static SDL_Renderer* renderer_;
    static Texture* selectSprite_; //!temp
    static SDL_Cursor* handCursor_;
    static SDL_Cursor* arrowCursor_;

    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize, const std::string mapFile);
    void loadMap(const std::string& mapFile);
    std::pair<int,int> getSizeOfMapFile(const std::string& mapFile);

    void updateNeighbors();
    void createSprite();
    void updateSelectedCell();
    bool isSelectableTroop(Cell* cell);

    void moveTroop(PlayableGround* from, PlayableGround* to);

    double ratio_ = 0;

    std::optional<Point> selectedCell_;
    bool hasSelection_ = false;

    std::vector<Player *> players_;

    std::unordered_set<Troop *> movedTroops_;
    Troop* selectedTroop_ = nullptr;
    PlayableGround* selectedTroopCell_ = nullptr;
    int selectedPlayerNum_ = 0;

    Point pos_;
    Size size_;
    Size spriteSize_;
    Texture* islandsCalc_ = nullptr;
    Texture* cellsCalc_ = nullptr;
    Texture* selectablesCalc_ = nullptr;
    Texture* fencesCalc_ = nullptr;
    Texture* elementsCalc_ = nullptr;
};

#endif
