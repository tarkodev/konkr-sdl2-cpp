#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <SDL.h>
#include <memory>
#include <utility>
#include "HexagonGrid.hpp"

/**
 * @brief Classe représentant une carte de jeu.
 *
 * Hérite de HexagonGrid<int> pour gérer la logique de la grille d'hexagones,
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, etc.).
 */
class GameMap : public HexagonGrid<int> {
public:
    GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::pair<int, int>& gridSize);
    GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::string mapFile);

    void addX(int x);
    void addY(int x);
    void setX(int x);
    void setY(int y);
    SDL_Point getPos() const;
    void setPos(SDL_Point& position);

    SDL_Rect getSize() const;
    void setProportionalSize(const SDL_Rect size);

    void handleEvent(SDL_Event &event);
    void draw();

private:
    void createSprite();
    void createHexagonSprite();
    void drawSprite();

    void selectHexagone();

    void drawNgon(const SDL_Color& color, int n, double rad,
                  const std::pair<double, double>& position, int width = 0) const;
    

    SDL_Renderer *renderer_;

    SDL_Rect hexagonSize_;
    double hexagonRadius_;
    double innerHexagonRadius_;

    std::optional<std::pair<int,int>> selectedHexagone_;
    bool hasSelection_ = false;

    SDL_Rect dest_;
    SDL_Rect spriteSize_;
    SDL_Texture* sprite_ = nullptr;
    SDL_Texture* hexagonSprite_ = nullptr;
};

#endif
