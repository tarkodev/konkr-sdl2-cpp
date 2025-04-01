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
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, zoom, etc.).
 */
class GameMap : public HexagonGrid<int> {
public:
    /**
     * @brief Constructeur de la GameMap.
     * @param renderer renderer processor.
     * @param gridSize Pair (width, height) de la grille.
     * @param size Pair (width, height) de la taille de texture.
     * @param position Pair (x, y) de la grille.
     * @param hexagonRadius Rayon des hexagones (pour la conversion et le dessin).
     * @param zoom Facteur de zoom initial.
     */
    GameMap(SDL_Renderer *renderer, const std::pair<int, int>& gridSize, const std::pair<int, int>& size, const std::pair<int, int>& position, double hexagonRadius, double zoom = 1.0);
    
    /**
     * @brief Constructeur de la GameMap.
     * @param renderer renderer processor.
     * @param mapFile File of game map.
     * @param size Pair (width, height) de la taille de texture.
     * @param position Pair (x, y) de la grille.
     * @param hexagonRadius Rayon des hexagones (pour la conversion et le dessin).
     * @param zoom Facteur de zoom initial.
     */
    GameMap(SDL_Renderer *renderer, const std::string mapFile, const std::pair<int, int>& size, const std::pair<int, int>& position, double hexagonRadius, double zoom = 1.0);

    /**
     * @brief Modifie le rayon des hexagones et invalide le sprite.
     */
    void setHexagonRadius(double hexagonRadius);

    /**
     * @brief Modifie le zoom et invalide le sprite.
     */
    void setZoom(double zoom);

    /**
     * @brief Modifie le zoom et invalide le sprite.
     */
    void zoom(double coef);

    /**
     * @brief Dessine la GameMap.
     */
    void draw();


    void handleEvent(SDL_Event &event);

private:
    /**
     * @brief Crée le sprite global de la carte.
     */
    void createSprite();

    /**
     * @brief Crée le sprite d'un hexagone.
     */
    void createHexagonSprite();

    /**
     * @brief Dessine un polygone (hexagone).
     */
    void drawNgon(const SDL_Color& color, int n, double rad,
                  const std::pair<double, double>& position, int width = 0) const;
    

    SDL_Renderer *renderer_;
    int width_;
    int height_;
    int x_;
    int y_;
    double hexagonRadius_;
    double zoom_;
    std::pair<int,int> selectedHexagone_ = {0, 0};
    std::pair<int,int> *moveOrigin_ = nullptr;
    bool hasSelection_ = false;

    SDL_Texture* sprite_;
    SDL_Texture* hexagonSprite_;
};

#endif
