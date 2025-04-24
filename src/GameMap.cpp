#include "GameMap.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Sea.hpp"
#include "Territory.hpp"
#include "Plain.hpp"
#include "RenderTargetGuard.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "DrawUtils.hpp"

#include <cmath>
#include <vector>
#include <iostream>


//! ? Renomer GameMap en CellGrid ?

SDL_Renderer* GameMap::renderer_ = nullptr;
Texture* GameMap::selectSprite_ = nullptr;


GameMap::GameMap(const Size size, const std::pair<int, int>& gridSize)
    : size_{size}, HexagonGrid<Cell*>(gridSize, nullptr)
{
    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            int v = std::rand() % 3;
            if (v == 0)
                set(col, row, new Territory());
            else if (v == 1)
                set(col, row, new Sea());
            else if (v == 2)
                set(col, row, new Plain());
        }
    }
    refresh();
}

GameMap::GameMap(const Size size, const std::string mapFile)
    : size_{size}, HexagonGrid<Cell*>(mapFile)
{
    refresh();
}

void GameMap::init(SDL_Renderer *renderer) {
    renderer_ = renderer;

    selectSprite_ = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha();
}


void GameMap::updateNeighbors() {
    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            if (row & 1) {
                get(row, col)->setNeighbors({
                    row > 0                                   ? get(row-1, col)   : nullptr,
                    col > 0                                   ? get(row, col-1)   : nullptr,
                    row+1 < getHeight()                       ? get(row+1, col)   : nullptr,
                    col+1 < getWidth() && row+1 < getHeight() ? get(row+1, col+1) : nullptr,
                    col+1 < getWidth()                        ? get(row, col+1)   : nullptr,
                    col+1 < getWidth() && row > 0             ? get(row-1, col+1) : nullptr,
                });
            } else {
                get(row, col)->setNeighbors({
                    col > 0 && row > 0                        ? get(row-1, col-1) : nullptr,
                    col > 0                                   ? get(row, col-1)   : nullptr,
                    col > 0 && row+1 < getHeight()            ? get(row+1, col-1) : nullptr,
                    row+1 < getHeight()                       ? get(row+1, col)   : nullptr,
                    col+1 < getWidth()                        ? get(row, col+1)   : nullptr,
                    row > 0                                   ? get(row-1, col)   : nullptr,
                });
            }
        }
    }
}

void GameMap::createSprite() {
    Size islandSize = Territory::getSpriteSize();
    double islandInnerRadius = Territory::getInnerRadius();
    double islandRadius = Territory::getRadius();

    auto [q, r] = HexagonUtils::offsetToAxial(getWidth()-1, getHeight()-1);
    auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandRadius);

    cx += islandInnerRadius + islandSize.getWidth() / 2.0;
    cy += islandRadius + islandSize.getHeight() / 2.0;

    spriteSize_ = {static_cast<int>(cx), static_cast<int>(cy)};
    setProportionalSize(spriteSize_);

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
    updateNeighbors();
    if (!islands_ || !cells_)
        createSprite();

    // Fond uniforme
    islands_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));


    Size islandSize = Territory::getSpriteSize();
    double islandInnerRadius = Territory::getInnerRadius();
    double islandRadius = Territory::getRadius();
    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            Cell* cell = get(row, col);

            // Calcul de la position de blit
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandRadius);
            auto pos = Point{
                static_cast<int>(cx + islandInnerRadius),
                static_cast<int>(cy + islandRadius)
            };

            if (auto* t = dynamic_cast<Territory*>(cell))
                t->Territory::display(islands_, pos);

            if (cell->getType() == Territory::TYPE) continue;

            if (auto* disp = dynamic_cast<Displayer*>(cell))
                disp->display(cells_, pos);


            /*
            // Dessine ile
            if (!isTerritory(cell)) continue;

            //! set les voisins dans Cell et getNeighbors;
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

            islandDisplayer_.display(islands_, Point{
                static_cast<int>(cx + islandInnerRadius_),
                static_cast<int>(cy + islandRadius_)
            }, neighbors);*/
        }
    }
}

Size GameMap::getSize() const {
    return size_;
}

void GameMap::setProportionalSize(const Size size) {
    double islandRadius = Territory::getRadius();

    // Get ratio
    double ratioW = static_cast<double>(size.getWidth()) / spriteSize_.getWidth();
    double ratioH = static_cast<double>(size.getHeight()) / spriteSize_.getHeight();
    double ratio = std::min(ratioW, ratioH);

    // Calculate proportionnal destination
    size_ = spriteSize_ * ratio;
    hexagonRadius_ = islandRadius * ratio;
    innerHexagonRadius_ = std::sqrt(3) * hexagonRadius_ / 2.0;
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
