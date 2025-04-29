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

/**
 * @brief Classe représentant une carte de jeu.
 *
 * Hérite de HexagonGrid<int> pour gérer la logique de la grille d'hexagons,
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, etc.).
 */
class GameMap : public HexagonGrid<Cell*> {
public:
    GameMap(const Size size, const std::pair<int, int>& gridSize);
    GameMap(const Size size, const std::string mapFile);

    Size getSize() const;
    void setProportionalSize(const Size size);
    void selectHexagon(const Point& pos);

    void refreshElements();
    void refreshSelectables();
    void refresh();

    bool hasTroopSelected() {return selectedTroopCell_ != nullptr;};

    void handleEvent(SDL_Event &event);
    void draw(const Point& pos);

    void endTurn();
    void test();

    static void init(SDL_Renderer* renderer);

private:
    static SDL_Renderer* renderer_;
    static Texture* selectSprite_; //!temp

    GameMap(const Size size, const std::pair<int, int>& gridSize, const std::string mapFile);
    void loadMap(const std::string& mapFile);
    std::pair<int,int> getSizeOfMapFile(const std::string& mapFile);

    void updateNeighbors();
    void createSprite();

    double cellRadius_; //! Peut être pas garder et enregistrer un ratio (taille_sprite/taille_reelle)
    double innerCellRadius_;

    std::optional<Point> selectedHexagon_;
    bool hasSelection_ = false;

    std::vector<Player *> players_;

    PlayableGround* selectedTroopCell_ = nullptr;
    int selectedPlayerNum_ = 0;

    Size size_;
    Size spriteSize_;
    Texture* islandsCalc_ = nullptr;
    Texture* cellsCalc_ = nullptr;
    Texture* selectablesCalc_ = nullptr;
    Texture* fencesCalc_ = nullptr;
    Texture* elementsCalc_ = nullptr;
};

#endif
