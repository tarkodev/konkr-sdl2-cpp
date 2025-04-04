#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "SDL.h"
#include <memory>
#include <utility>
#include "HexagonGrid.hpp"
#include "Cell.hpp"

/**
 * @brief Classe représentant une carte de jeu.
 *
 * Hérite de HexagonGrid<int> pour gérer la logique de la grille d'hexagons,
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, etc.).
 */
class GameMap : public HexagonGrid<Cell*> {
public:
    GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::pair<int, int>& gridSize, const SDL_Rect& hexagonSize);
    GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::string mapFile, const SDL_Rect& hexagonSize);


    SDL_Rect getSize() const;
    void setProportionalSize(const SDL_Rect size);
    void selectHexagon(SDL_Point pos);
    void refresh();

    void handleEvent(SDL_Event &event);
    void draw(SDL_Point& pos);

private:
    void createSprite();

    void drawNgon(const SDL_Color& color, int n, double rad,
                  const std::pair<double, double>& position, int width = 0) const;
    

    SDL_Renderer *renderer_;

    SDL_Rect hexagonSize_;
    double hexagonSpriteRadius_;
    double hexagonRadius_;
    double innerHexagonRadius_;

    std::optional<std::pair<int,int>> selectedHexagon_;
    bool hasSelection_ = false;

    SDL_Rect size_;
    SDL_Rect spriteSize_;
    SDL_Texture* sprite_ = nullptr;
};

#endif
