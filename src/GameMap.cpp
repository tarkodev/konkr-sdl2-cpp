#include "GameMap.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Sea.hpp"
#include "Territory.hpp"
#include "RenderTargetGuard.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "DrawUtils.hpp"

#include <cmath>
#include <vector>
#include <iostream>


SDL_Renderer* GameMap::renderer_ = nullptr;

Texture* GameMap::selectSprite_ = nullptr;
Texture* GameMap::islandSprite_ = nullptr;
Texture* GameMap::islandLink1Sprite_ = nullptr;
Texture* GameMap::islandLink2Sprite_ = nullptr;
Texture* GameMap::islandLink3Sprite_ = nullptr;
Texture* GameMap::islandLink4Sprite_ = nullptr;

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

void GameMap::init(SDL_Renderer *renderer) {
    renderer_ = renderer;

    islandSprite_ = new Texture(renderer_, "../assets/img/hexagon.png");
    islandSprite_->convertAlpha();

    islandLink1Sprite_ = new Texture(renderer_, "../assets/img/hexagon_star1.png");
    islandLink1Sprite_->convertAlpha();

    islandLink2Sprite_ = new Texture(renderer_, "../assets/img/hexagon_star2.png");
    islandLink2Sprite_->convertAlpha();

    islandLink3Sprite_ = new Texture(renderer_, "../assets/img/hexagon_star3.png");
    islandLink3Sprite_->convertAlpha();

    islandLink4Sprite_ = new Texture(renderer_, "../assets/img/hexagon_star4.png");
    islandLink4Sprite_->convertAlpha();

    selectSprite_ = new Texture(renderer_, "../assets/img/plate.png");
    selectSprite_->convertAlpha();

    islandSpriteInnerRadius_ = islandSprite_->getWidth() / 2;
    islandSpriteRadius_ = islandSpriteInnerRadius_ * 2 / std::sqrt(3);
}


void GameMap::createSprite() {
    auto [q, r] = HexagonUtils::offsetToAxial(getWidth()-1, getHeight()-1);
    auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandSpriteRadius_);

    cx += islandSpriteInnerRadius_ + islandSprite_->getWidth() / 2.0;
    cy += islandSpriteRadius_ + islandSprite_->getHeight() / 2.0;

    spriteSize_ = {
        static_cast<int>(cx),
        static_cast<int>(cy),
    };
    setProportionalSize(size_);

    if (islands_) {
        delete islands_;
        islands_ = nullptr;
    }

    islands_ = new Texture(renderer_, spriteSize_);
    islands_->convertAlpha();

    if (cells_) {
        delete cells_;
        cells_ = nullptr;
    }

    cells_ = new Texture(renderer_, spriteSize_);
    cells_->convertAlpha();
}

void GameMap::refresh()
{
    if (!islands_ || !cells_)
        createSprite();

    // Fond uniforme
    islands_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            Cell* cell = get(row, col);

            // Calcul de la position de blit
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandSpriteRadius_);

            cell->draw(cells_, Point{
                static_cast<int>(cx + islandSpriteInnerRadius_),
                static_cast<int>(cy + islandSpriteRadius_)
            });

            // Dessine ile
            if (!isTerritory(cell)) continue;

            islands_->blit(islandSprite_, Point{
                static_cast<int>(cx + islandSpriteInnerRadius_ - islandSprite_->getWidth() / 2.0),
                static_cast<int>(cy + islandSpriteRadius_ - islandSprite_->getHeight() / 2.0)
            });

            std::vector<bool> neighbors(4);
            if (row & 1) {
                neighbors = {
                    row > 0 && isTerritory(get(row-1, col)),
                    col > 0 && isTerritory(get(row, col-1)),
                    row+1 < getHeight() && isTerritory(get(row+1, col)),
                    row+1 < getHeight() && col+1 < getWidth() && isTerritory(get(row+1, col+1))
                };
            } else {
                neighbors = {
                    row > 0 && col > 0 && isTerritory(get(row-1, col-1)),
                    col > 0 && isTerritory(get(row, col-1)),
                    row+1 < getHeight() && col > 0 && isTerritory(get(row+1, col-1)),
                    row+1 < getHeight() && isTerritory(get(row+1, col))
                };
            }

            if (neighbors[0]) {
                Texture* linkSprite = neighbors[1] ? islandLink1Sprite_ : islandLink2Sprite_;
                islands_->blit(linkSprite, Point{
                    static_cast<int>(cx + islandSpriteInnerRadius_ / 2 - linkSprite->getWidth() / 2.0),
                    static_cast<int>(cy + islandSpriteRadius_ * 0.25 - linkSprite->getHeight() / 2.0)
                });
            }
            if (neighbors[1]) {
                Texture* linkSprite = neighbors[2] ? islandLink1Sprite_ : islandLink3Sprite_;
                islands_->blit(linkSprite, Point{
                    static_cast<int>(cx - linkSprite->getWidth() / 2.0),
                    static_cast<int>(cy + islandSpriteRadius_ - linkSprite->getHeight() / 2.0)
                });
            }
            if (neighbors[2]) {
                Texture* linkSprite = neighbors[3] ? islandLink1Sprite_ : islandLink4Sprite_;
                islands_->blit(linkSprite, Point{
                    static_cast<int>(cx + islandSpriteInnerRadius_ / 2 - linkSprite->getWidth() / 2.0),
                    static_cast<int>(cy + islandSpriteRadius_ * 1.75 - linkSprite->getHeight() / 2.0)
                });
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

void GameMap::draw(const Point& pos)
{
    if (!islands_)
        refresh();

    Rect dest = {pos, size_};
    SDL_RenderCopy(renderer_, islands_->get(), nullptr, &dest.get());
    SDL_RenderCopy(renderer_, cells_->get(), nullptr, &dest.get());

    if (selectedHexagon_.has_value()) {
        auto [q, r] = HexagonUtils::offsetToAxial(selectedHexagon_->getX(), selectedHexagon_->getY());
        auto [x, y] = HexagonUtils::axialToPixel(q, r, hexagonRadius_);
        x += pos.getX() + innerHexagonRadius_;
        y += pos.getY() + hexagonRadius_;

        double ratio = static_cast<double>(dest.getWidth()) / islands_->getWidth();
        int w = static_cast<int>(ratio * selectSprite_->getWidth());
        int h = static_cast<int>(ratio * selectSprite_->getHeight());

        SDL_RenderCopy(renderer_, selectSprite_->get(), nullptr, new SDL_Rect{static_cast<int>(x -  w/2), static_cast<int>(y - h/2), w, h});

        // Dessiner un contour épais en jaune
        //!drawNgon(ColorUtils::YELLOW, 6, hexagonRadius_, {x, y}, 3);
    }
}

void GameMap::test() {
    if (!selectedHexagon_.has_value()) return;

    set(selectedHexagon_->getY(), selectedHexagon_->getX(), new Sea());
    refresh();
}

void GameMap::selectHexagon(const Point& pos) {
    int relX = pos.getX() - innerHexagonRadius_;
    int relY = pos.getY() - hexagonRadius_;

    auto [q, r] = HexagonUtils::pixelToAxial(relX, relY, hexagonRadius_);
    auto [x, y] = HexagonUtils::axialToOffset(q, r);
    
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
        selectedHexagon_.reset();
    else
        selectedHexagon_ = {x, y};
}

void GameMap::handleEvent(SDL_Event &event) {}
