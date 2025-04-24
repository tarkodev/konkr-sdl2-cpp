#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "SDL.h"
#include <memory>
#include <utility>
#include <optional>
#include "HexagonGrid.hpp"
#include "Texture.hpp"
#include "Cell.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "Size.hpp"

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
    GameMap(const Size size, const std::pair<int, int>& gridSize, const std::string mapFile);


    Size getSize() const;
    void setProportionalSize(const Size size);
    void selectHexagon(const Point& pos);
    void refresh();

    void handleEvent(SDL_Event &event);
    void draw(const Point& pos);

    void test();

    static void init(SDL_Renderer* renderer);

private:
    static SDL_Renderer* renderer_;
    static Texture* selectSprite_;

    std::pair<int,int> detectMapSize(const std::string& mapFile);
    void updateNeighbors();
    void createSprite();

    double hexagonRadius_;
    double innerHexagonRadius_;

    std::optional<Point> selectedHexagon_;
    bool hasSelection_ = false;

    Size size_;
    Size spriteSize_;
    Texture* islands_ = nullptr;
    Texture* cells_ = nullptr;
    
};

#endif
