#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "SDL.h"
#include <memory>
#include <utility>
#include <optional>
#include "HexagonGrid.hpp"
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


    Size getSize() const;
    void setProportionalSize(const Size size);
    void selectHexagon(const Point& pos);
    void refresh();

    void handleEvent(SDL_Event &event);
    void draw(const Point& pos);
    bool isTerritory(Cell *cell) const;

    void test();

    static void init(SDL_Renderer* renderer);

private:
    static SDL_Renderer* renderer_;

    static Texture* selectSprite_;
    static Texture* islandSprite_;
    static Texture* islandLink1Sprite_;
    static Texture* islandLink2Sprite_;
    static Texture* islandLink3Sprite_;
    static Texture* islandLink4Sprite_;

    static double islandSpriteRadius_;
    static double islandSpriteInnerRadius_;

    void createSprite();

    void drawNgon(const SDL_Color& color, int n, double rad,
                  const std::pair<double, double>& position, int width = 0) const;

    double hexagonRadius_;
    double innerHexagonRadius_;

    std::optional<Point> selectedHexagon_;
    bool hasSelection_ = false;

    Size size_;
    Size spriteSize_;
    Texture* sprite_ = nullptr;
    
};

#endif
