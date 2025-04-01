#include "GameMap.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <cmath>
#include <vector>

// Définition de couleurs constantes pour le rendu
namespace {
    const SDL_Color BROWN  = {81, 62, 62, 255};
    const SDL_Color YELLOW = {252, 242, 0, 255};
}

GameMap::GameMap(SDL_Renderer *renderer, const std::pair<int, int>& gridSize, const std::pair<int, int>& size, const std::pair<int, int>& position, double hexagonRadius, double zoom)
    : renderer_(renderer), HexagonGrid<int>(gridSize, hexagonRadius, 0),
      width_(size.first), height_(size.second), x_(position.first), y_(position.second), 
      hexagonRadius_(hexagonRadius), zoom_(zoom), sprite_(nullptr), hexagonSprite_(nullptr) {}


GameMap::GameMap(SDL_Renderer *renderer, const std::string mapFile, const std::pair<int, int>& size, const std::pair<int, int>& position, double hexagonRadius, double zoom)
    : renderer_(renderer), HexagonGrid<int>(mapFile, hexagonRadius),
      width_(size.first), height_(size.second), x_(position.first), y_(position.second), 
      hexagonRadius_(hexagonRadius), zoom_(zoom), sprite_(nullptr), hexagonSprite_(nullptr) {}

void GameMap::createSprite()
{
    // Si un sprite existe déjà, le détruire
    if (sprite_) {
        SDL_DestroyTexture(sprite_);
        sprite_ = nullptr;
    }

    // Créer la texture du sprite (dimensions basées sur la grille)
    sprite_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width_, height_);
    this->createHexagonSprite();
}

void GameMap::createHexagonSprite()
{
    // Calculer la taille du sprite d'hexagone
    const int hexagonSize = 2 * static_cast<int>(std::floor(hexagonRadius_ * zoom_)) + 2;

    if (hexagonSprite_) {
        SDL_DestroyTexture(hexagonSprite_);
        hexagonSprite_ = nullptr;
    }

    hexagonSprite_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, hexagonSize, hexagonSize);
    SDL_SetTextureBlendMode(hexagonSprite_, SDL_BLENDMODE_BLEND);

    // Initialiser la texture : effacer avec transparence
    SDL_SetRenderTarget(renderer_, hexagonSprite_);
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);

    // Dessiner l'hexagone dans la texture
    std::vector<Sint16> vx(6), vy(6);
    const double center = hexagonSize / 2.0;
    for (int i = 0; i < 6; ++i)
    {
        double angle = M_PI_2 + (i / 6.0) * (2 * M_PI);
        vx[i] = static_cast<Sint16>(center + std::cos(angle) * hexagonRadius_ * zoom_);
        vy[i] = static_cast<Sint16>(center + std::sin(angle) * hexagonRadius_ * zoom_);
    }
    filledPolygonRGBA(renderer_, vx.data(), vy.data(), 6, 255, 255, 255, 255);

    SDL_SetRenderTarget(renderer_, nullptr);
}

void GameMap::setHexagonRadius(double hexagonRadius)
{
    hexagonRadius_ = hexagonRadius;
    // Invalider le sprite pour forcer une recréation lors du prochain dessin
    if (sprite_) {
        SDL_DestroyTexture(sprite_);
        sprite_ = nullptr;
    }
}

void GameMap::setZoom(double zoom)
{
    zoom_ = zoom;
    if (sprite_) {
        SDL_DestroyTexture(sprite_);
        sprite_ = nullptr;
    }
}

void GameMap::zoom(double coef = 1.1)
{
    zoom_ *= coef;
    if (sprite_) {
        SDL_DestroyTexture(sprite_);
        sprite_ = nullptr;
    }
}

void GameMap::drawNgon(const SDL_Color &color, int n, double rad,
                       const std::pair<double, double> &position, int width) const
{
    std::vector<Sint16> vx(n), vy(n);
    for (int i = 0; i < n; ++i)
    {
        double angle = M_PI_2 + (static_cast<double>(i) / n) * (2 * M_PI);
        vx[i] = static_cast<Sint16>(std::round(std::cos(angle) * rad + position.first));
        vy[i] = static_cast<Sint16>(std::round(std::sin(angle) * rad + position.second));
    }
    polygonRGBA(renderer_, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
    for (int w = 1; w < width; ++w)
        polygonRGBA(renderer_, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
}

void GameMap::draw()
{
    if (!sprite_)
        createSprite();

    // Définir la couleur de modulation pour le sprite hexagonal
    SDL_SetTextureColorMod(hexagonSprite_, BROWN.r, BROWN.g, BROWN.b);
    SDL_SetTextureAlphaMod(hexagonSprite_, BROWN.a);

    // Calculer le rectangle source du sprite (taille basée sur le rayon)
    const int spriteSize = 2 * static_cast<int>(std::floor(hexagonRadius_ * zoom_)) + 2;
    const SDL_Rect srcRect = {0, 0, spriteSize, spriteSize};

    // Dessiner chaque hexagone de la grille
    for (int row = 0; row < getHeight(); ++row)
    {
        for (int col = 0; col < getWidth(); ++col)
        {
            // Conversion des coordonnées offset en coordonnées axiales via la méthode statique héritée
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [x, y] = HexagonUtils::hexToPixel(q, r, hexagonRadius_ * zoom_);
            x += x_;
            y += y_;

            SDL_Rect destRect = {
                static_cast<int>(x - hexagonRadius_ * zoom_ - 1),
                static_cast<int>(y - hexagonRadius_ * zoom_ - 1),
                srcRect.w,
                srcRect.h
            };
            SDL_RenderCopy(renderer_, hexagonSprite_, &srcRect, &destRect);
        }
    }

    // Dessin de la sélection, si présente
    if (hasSelection_)
    {
        auto [x, y] = HexagonUtils::hexToPixel(selectedHexagone_.first, selectedHexagone_.second, hexagonRadius_ * zoom_);
        x += x_;
        y += y_;

        // Dessiner un contour épais en jaune
        drawNgon(YELLOW, 6, hexagonRadius_ * zoom_, {x, y}, 3);
    }
}

void GameMap::handleEvent(SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN)
        moveOrigin_ = new std::pair<int, int>(event.motion.x, event.motion.y);
    else if (event.type == SDL_MOUSEBUTTONUP)
        moveOrigin_ = nullptr;
    else if (event.type == SDL_MOUSEMOTION) {
        if (moveOrigin_) {
            x_ += event.motion.x - moveOrigin_->first;
            y_ += event.motion.y - moveOrigin_->second;
            moveOrigin_ = new std::pair<int, int>(event.motion.x, event.motion.y);
        } else {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            
            // Coordonnées relatives
            double relX = mouseX - x_;
            double relY = mouseY - y_;
            selectedHexagone_ = HexagonUtils::pixelToHex(relX, relY, hexagonRadius_ * zoom_);
            hasSelection_ = true;
        }
    } else if (event.type == SDL_MOUSEWHEEL) {
        // Ajuste le rayon et recrée la grille si besoin
        this->zoom((event.wheel.preciseY > 0) ? 1.1 : 0.9);
    }
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT)
            x_ += 10;
        else if (event.key.keysym.sym == SDLK_RIGHT)
            x_ -= 10;
        else if (event.key.keysym.sym == SDLK_UP)
            y_ += 10;
        else if (event.key.keysym.sym == SDLK_DOWN) {
            y_ -= 10;
            
        }
    }
}
