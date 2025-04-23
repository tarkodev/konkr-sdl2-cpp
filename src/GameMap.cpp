#include "GameMap.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Territory.hpp"
#include "RenderTargetGuard.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "DrawUtils.hpp"

#include <cmath>
#include <vector>
#include <iostream>


SDL_Renderer* GameMap::renderer_ = nullptr;
Texture* GameMap::islandSprite_ = nullptr;
double GameMap::islandSpriteRadius_ = 0;
double GameMap::islandSpriteInnerRadius_ = 0;


GameMap::GameMap(const Size size, const std::pair<int, int>& gridSize)
    : size_{size}, HexagonGrid<Cell*>(gridSize, new Territory())
{
    refresh();
}

GameMap::GameMap(const Size size, const std::string mapFile)
    : size_{size}, HexagonGrid<Cell*>(mapFile)
{
    refresh();
}

void GameMap::init(SDL_Renderer *renderer, Texture* islandSprite, double islandSpriteRadius) {
    renderer_ = renderer;
    islandSprite_ = islandSprite;
    islandSpriteRadius_ = islandSpriteRadius;
    islandSpriteInnerRadius_ = std::sqrt(3) * islandSpriteRadius_ / 2.0;
}


void GameMap::createSprite() {
    if (sprite_) return;

    spriteSize_ = {
        static_cast<int>(2 * islandSpriteInnerRadius_ * (getWidth() + 0.5)), //! + (spriteWidth - innerHexagonSize)
        static_cast<int>(islandSpriteRadius_ * (1.5 * getHeight() + 0.5)), //! + (spriteHeight - hexagonRadius*2)
    };
    setProportionalSize(size_);

    sprite_ = new Texture(renderer_, spriteSize_);
    sprite_->convertAlpha();
}

void GameMap::refresh()
{
    createSprite();

    // Fond uniforme
    sprite_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    for (int row = 0; row < getHeight(); ++row) {
        for (int col = 0; col < getWidth(); ++col) {
            Cell* cell = get(row, col);
            if (!isTerritory(cell)) continue;

            // Calcul de la position de blit
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandSpriteRadius_);
            Rect dest{
                Point{
                    int(cx + islandSpriteInnerRadius_ - islandSprite_->getWidth()/2.0),
                    int(cy + islandSpriteRadius_    - islandSprite_->getHeight()/2.0)
                },
                islandSprite_->getSize()
            };

            // --- Préparer la liste "neighbors" Commune ---
            std::vector<bool> neighbors(6);
            if (row & 1) {
                neighbors = {
                    row+1<getHeight()                     && isTerritory(get(row+1, col)),
                    col>0                                 && isTerritory(get(row,   col-1)),
                    row>0                                 && isTerritory(get(row-1, col)),
                    row>0 && col+1<getWidth()             && isTerritory(get(row-1, col+1)),
                    col+1<getWidth()                      && isTerritory(get(row,   col+1)),
                    row+1<getHeight() && col+1<getWidth() && isTerritory(get(row+1, col+1))
                };
            } else {
                neighbors = {
                    row+1<getHeight() && col>0       && isTerritory(get(row+1, col-1)),
                    col>0                            && isTerritory(get(row,   col-1)),
                    row>0 && col>0                   && isTerritory(get(row-1, col-1)),
                    row>0                            && isTerritory(get(row-1, col)),
                    col+1<getWidth()                 && isTerritory(get(row,   col+1)),
                    row+1<getHeight()                && isTerritory(get(row+1, col))
                };
            }


            {
                Texture* tempTex = islandSprite_->copy();
                RenderTargetGuard tg(renderer_, tempTex);
                double rad = std::sqrt(std::pow(islandSprite_->getWidth(), 2) + std::pow(islandSprite_->getHeight(), 2)) / 2.0;

                // Quartiers jaunes
                /*drawQuarters(
                    ColorUtils::BROWN2,
                    rad,
                    { tempTex->getWidth()/2.0, tempTex->getHeight()/2.0 },
                    neighbors
                );*/

                // Blit de tempTex dans la texture finale
                sprite_->blit(tempTex, dest);
            }
        }
    }
}

bool GameMap::isTerritory(Cell *cell) const {
    return dynamic_cast<Territory*>(cell);
}


Size GameMap::getSize() const {
    return size_;
}

void GameMap::setProportionalSize(const Size size) {
    // Get ratio
    double ratioW = static_cast<double>(size.getWidth()) / spriteSize_.getWidth();
    double ratioH = static_cast<double>(size.getHeight()) / spriteSize_.getHeight();
    double ratio = std::min(ratioW, ratioH);

    // Calculate proportionnal destination
    size_ = spriteSize_ * ratio;
    hexagonRadius_ = islandSpriteRadius_ * ratio;
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


void GameMap::drawQuarters(const SDL_Color &color, double rad,
    const std::pair<double, double> &position,
    const std::vector<bool>& sectors) const
{
    //! temp
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
    int n = sectors.size();

    // Calcul des coordonnées des sommets du polygone.
    std::vector<Sint16> vx(n), vy(n);
    for (int i = 0; i < n; ++i) {
        double angle = M_PI_2 + (static_cast<double>(i) / n) * (2 * M_PI);
        vx[i] = static_cast<Sint16>(std::round(std::cos(angle) * rad + position.first));
        vy[i] = static_cast<Sint16>(std::round(std::sin(angle) * rad + position.second));
    }

    // Calcul des coordonnées du centre
    Sint16 centerX = static_cast<Sint16>(std::round(position.first));
    Sint16 centerY = static_cast<Sint16>(std::round(position.second));

    // Pour chaque secteur à remplir (indiqué par un booléen true)
    for (int i = 0; i < n; ++i)
    {
        if (!sectors[i]) continue;

        int next = (i + 1) % n; // Le sommet suivant, avec wrap-around
        DrawUtils::fillTriangle(renderer_, SDL_Point{centerX, centerY},
            SDL_Point{vx[i], vy[i]},
            SDL_Point{vx[next], vy[next]},
                color);
    }

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}




void GameMap::draw(const Point& pos)
{
    if (!sprite_)
        refresh();

    Rect dest = {pos, size_};
    SDL_RenderCopy(renderer_, sprite_->get(), nullptr, &dest.get());


    /*
    if (selectedHexagon_.has_value()) {
        std::pair<int,int> hexagon = selectedHexagon_.value();
        auto [x, y] = HexagonUtils::axialToPixel(hexagon.first, hexagon.second, hexagonRadius_);
        x += pos.x + innerHexagonRadius_;
        y += pos.y + hexagonRadius_;

        // Dessiner un contour épais en jaune
        drawNgon(ColorUtils::YELLOW, 6, hexagonRadius_, {x, y}, 3);
    }
    */
}

void GameMap::selectHexagon(const Point& pos) {
    int relX = pos.getX() - innerHexagonRadius_;
    int relY = pos.getY() - hexagonRadius_;
    auto [q, r] = HexagonUtils::pixelToAxial(relX, relY, hexagonRadius_);
    auto [x, y] = HexagonUtils::axialToOffset(q, r);
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
        selectedHexagon_.reset();
    else
        selectedHexagon_ = {q, r};
}

void GameMap::handleEvent(SDL_Event &event) {}
