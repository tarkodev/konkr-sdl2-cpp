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
Texture* GameMap::selectSprite_ = nullptr;
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

void GameMap::init(SDL_Renderer *renderer, Texture* selectSprite, Texture* islandSprite, double islandSpriteRadius) {
    renderer_ = renderer;
    selectSprite_ = selectSprite;
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

    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            Cell* cell = get(row, col);
            if (!isTerritory(cell)) continue;

            // Calcul de la position de blit
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandSpriteRadius_);
            Point dest{
                int(cx + islandSpriteInnerRadius_ - islandSprite_->getWidth()/2.0),
                int(cy + islandSpriteRadius_    - islandSprite_->getHeight()/2.0)
            };

            sprite_->blit(islandSprite_, dest);
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
    if (!sprite_)
        refresh();

    Rect dest = {pos, size_};
    SDL_RenderCopy(renderer_, sprite_->get(), nullptr, &dest.get());

    if (selectedHexagon_.has_value()) {
        auto [x, y] = HexagonUtils::axialToPixel(selectedHexagon_->getX(), selectedHexagon_->getY(), hexagonRadius_);
        x += pos.getX() + innerHexagonRadius_;
        y += pos.getY() + hexagonRadius_;

        double ratio = static_cast<double>(dest.getWidth()) / sprite_->getWidth();
        int w = static_cast<int>(ratio * selectSprite_->getWidth());
        int h = static_cast<int>(ratio * selectSprite_->getHeight());

        SDL_RenderCopy(renderer_, selectSprite_->get(), nullptr, new SDL_Rect{static_cast<int>(x -  w/2), static_cast<int>(y - h/2), w, h});

        // Dessiner un contour épais en jaune
        //!drawNgon(ColorUtils::YELLOW, 6, hexagonRadius_, {x, y}, 3);
    }
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
