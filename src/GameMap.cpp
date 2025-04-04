#include "GameMap.hpp"
#include "DrawUtils.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Cell.hpp"

#include <cmath>
#include <vector>
#include <iostream>

// Définition de couleurs constantes pour le rendu
namespace {
    const SDL_Color BROWN  = {81, 62, 62, 255};
    const SDL_Color YELLOW = {252, 242, 0, 255};
}


GameMap::GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::pair<int, int>& gridSize, const SDL_Rect& hexagonSize)
    : renderer_(renderer), size_{position.x, position.y, size.w, size.h}, HexagonGrid<Cell*>(gridSize, new Cell(renderer, nullptr)), hexagonSize_(hexagonSize)
{
    refresh();
}

GameMap::GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::string mapFile, const SDL_Rect& hexagonSize)
    : renderer_(renderer), size_{position.x, position.y, size.w, size.h}, HexagonGrid<Cell*>(mapFile), hexagonSize_(hexagonSize)
{
    refresh();
}


void GameMap::createSprite() {
    double hexagonRadius = hexagonSize_.h / 2.0 - 1;
    int w = (getWidth() - 1);
    int h = (getHeight() - 1);
    
    spriteSize_ = {
        0, 0,
        static_cast<int>(hexagonSize_.w + hexagonRadius * std::sqrt(3) * (w + (h & 1) / 2.0)),
        static_cast<int>(hexagonSize_.h + hexagonRadius * (3.0 / 2.0) * h)
    };
    setProportionalSize(size_);

    sprite_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, spriteSize_.w, spriteSize_.h);
    if (!sprite_)
        throw std::runtime_error("Échec lors de la génération de la map: " + std::string(SDL_GetError()));

    SDL_SetTextureBlendMode(sprite_, SDL_BLENDMODE_BLEND);
}

void GameMap::refresh()
{
    // Create surface of sprite
    if (!sprite_)
        createSprite();

    // Target sprite and reset draw
    SDL_SetRenderTarget(renderer_, sprite_);
    DrawUtils::SetRenderDrawColor(renderer_, ColorUtils::toTransparent(ColorUtils::SEABLUE)); //! passer la couleur en paramètre background
    SDL_RenderClear(renderer_);

    // Dessiner chaque hexagon de la grille
    double hexagonRadius = hexagonSize_.h / 2.0 - 1;
    SDL_Point hexagonPos;
    Cell *cell;
    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [x, y] = HexagonUtils::axialToPixel(q, r, hexagonRadius);

            hexagonPos = {
                static_cast<int>(x),
                static_cast<int>(y)
            };

            cell = get(row, col);
            if (cell) cell->draw(hexagonPos);
        }
    }

    SDL_SetRenderTarget(renderer_, nullptr);
}


SDL_Rect GameMap::getSize() const {
    return {0, 0, size_.w, size_.h};
}

void GameMap::setProportionalSize(const SDL_Rect size) {
    // Get ratio
    double ratioW = static_cast<double>(size.w) / spriteSize_.w;
    double ratioH = static_cast<double>(size.h) / spriteSize_.h;
    double ratio = std::min(ratioW, ratioH);

    // Calculate proportionnal destination
    int last_w = size_.w;
    int last_h = size_.h;
    size_.w = static_cast<int>(spriteSize_.w * ratio);
    size_.h = static_cast<int>(spriteSize_.h * ratio);

    hexagonRadius_ = static_cast<double>(size_.h) / spriteSize_.h * (hexagonSize_.h / 2.0 - 1);
    innerHexagonRadius_ = std::sqrt(3) * hexagonRadius_ / 2.0;
}


void GameMap::drawNgon(const SDL_Color &color, int n, double rad,
                       const std::pair<double, double> &position, int width) const
{
    std::vector<Sint16> vx(n), vy(n);
    for (int i = 0; i < n; ++i) {
        double angle = M_PI_2 + (static_cast<double>(i) / n) * (2 * M_PI);
        vx[i] = static_cast<Sint16>(std::round(std::cos(angle) * rad + position.first));
        vy[i] = static_cast<Sint16>(std::round(std::sin(angle) * rad + position.second));
    }

    polygonRGBA(renderer_, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
    for (int w = 1; w < width; ++w)
        polygonRGBA(renderer_, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
}

void GameMap::draw(SDL_Point& pos)
{
    if (!sprite_)
        refresh();

    SDL_Rect dest = {
        pos.x,
        pos.y,
        size_.w,
        size_.h
    };

    SDL_RenderCopy(renderer_, sprite_, &spriteSize_, &dest);

    if (selectedHexagon_.has_value()) {
        std::pair<int,int> hexagon = selectedHexagon_.value();
        auto [x, y] = HexagonUtils::axialToPixel(hexagon.first, hexagon.second, hexagonRadius_);
        x += pos.x + innerHexagonRadius_;
        y += pos.y + hexagonRadius_;

        // Dessiner un contour épais en jaune
        drawNgon(YELLOW, 6, hexagonRadius_, {x, y}, 3);
    }
}

void GameMap::selectHexagon(SDL_Point pos) {
    int relX = pos.x - innerHexagonRadius_;
    int relY = pos.y - hexagonRadius_;
    auto [q, r] = HexagonUtils::pixelToAxial(relX, relY, hexagonRadius_);
    auto [x, y] = HexagonUtils::axialToOffset(q, r);
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
        selectedHexagon_.reset();
    else
        selectedHexagon_ = {q, r};
}

void GameMap::handleEvent(SDL_Event &event) {}
