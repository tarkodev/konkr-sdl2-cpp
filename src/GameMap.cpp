#include "GameMap.hpp"
#include "DrawUtils.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"

#include <cmath>
#include <vector>
#include <iostream>

//! temp
SDL_Rect getTextureSize(SDL_Texture *texture) {
    SDL_Rect size = {};
    SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);
    return size;
}

// Définition de couleurs constantes pour le rendu
namespace {
    const SDL_Color BROWN  = {81, 62, 62, 255};
    const SDL_Color YELLOW = {252, 242, 0, 255};
}


GameMap::GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::pair<int, int>& gridSize)
    : renderer_(renderer), dest_{position.x, position.y, size.w, size.h}, HexagonGrid<int>(gridSize, 0)
{}

GameMap::GameMap(SDL_Renderer *renderer, const SDL_Point& position, const SDL_Rect& size, const std::string mapFile)
    : renderer_(renderer), dest_{position.x, position.y, size.w, size.h}, HexagonGrid<int>(mapFile)
{}


void GameMap::createSprite() {
    double hexagonRadius = hexagonSize_.h / 2 - 1;

    int w = (getWidth() - 1);
    int h = (getHeight() - 1);
    
    spriteSize_ = {
        0, 0,
        static_cast<int>(hexagonSize_.w + hexagonRadius * std::sqrt(3) * (w + (h & 1) / 2.0)),
        static_cast<int>(hexagonSize_.h + hexagonRadius * (3.0 / 2.0) * h)
    };
    setProportionalSize(dest_);

    sprite_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, spriteSize_.w, spriteSize_.h);
    if (!sprite_)
        throw std::runtime_error("Échec lors de la génération de la map: " + static_cast<std::string>(SDL_GetError()));

    SDL_SetTextureBlendMode(sprite_, SDL_BLENDMODE_BLEND);
}

void GameMap::createHexagonSprite()
{
    // Load texture
    hexagonSprite_ = IMG_LoadTexture(renderer_, "../assets/img/hexagon.png");
    if (!hexagonSprite_)
        throw std::runtime_error("Échec du chargement de la texture des hexagones.");

    hexagonSize_ = getTextureSize(hexagonSprite_);
}

void GameMap::drawSprite()
{
    // Create surface of sprite
    if (!hexagonSprite_)
        createHexagonSprite();
    if (!sprite_)
        createSprite();

    // Target sprite and reset draw
    SDL_SetRenderTarget(renderer_, sprite_);
    SetRenderDrawColor(renderer_, toTransparent(CLR_SEABLUE));
    SDL_RenderClear(renderer_);

    // Dessiner chaque hexagone de la grille
    SDL_Rect destRect;
    double hexagonRadius = hexagonSize_.h / 2 - 1;
    for (int row = 0; row < getHeight(); row++) {
        for (int col = 0; col < getWidth(); col++) {
            auto [q, r] = HexagonUtils::offsetToAxial(col, row);
            auto [x, y] = HexagonUtils::axialToPixel(q, r, hexagonRadius);

            destRect = {
                static_cast<int>(x),
                static_cast<int>(y),
                hexagonSize_.w,
                hexagonSize_.h
            };
            
            SDL_RenderCopy(renderer_, hexagonSprite_, &hexagonSize_, &destRect);
        }
    }

    SDL_SetRenderTarget(renderer_, nullptr);
}


SDL_Rect GameMap::getSize() const {
    return {0, 0, dest_.w, dest_.h};
}

void GameMap::setProportionalSize(const SDL_Rect size) {
    // Get ratio
    double ratioW = static_cast<double>(size.w) / spriteSize_.w;
    double ratioH = static_cast<double>(size.h) / spriteSize_.h;
    double ratio = std::min(ratioW, ratioH);

    // Calculate proportionnal destination
    int last_w = dest_.w;
    int last_h = dest_.h;
    dest_.w = static_cast<int>(spriteSize_.w * ratio);
    dest_.h = static_cast<int>(spriteSize_.h * ratio);
    dest_.x += (last_w - dest_.w) / 2;
    dest_.y += (last_h - dest_.h) / 2;

    hexagonRadius_ = static_cast<double>(dest_.h) / spriteSize_.h * (hexagonSize_.h / 2.0 - 1);
    innerHexagonRadius_ = std::sqrt(3) * hexagonRadius_ / 2.0;
}


SDL_Point GameMap::getPos() const {
    return {dest_.x, dest_.y};
}

void GameMap::addX(int x) {
    dest_.x += x;
}

void GameMap::addY(int y) {
    dest_.y += y;
}

void GameMap::setX(int x) {
    dest_.x = x;
}

void GameMap::setY(int y) {
    dest_.y = y;
}

void GameMap::setPos(SDL_Point& position) {
    dest_.x = position.x;
    dest_.y = position.y;
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

void GameMap::draw()
{
    if (!sprite_)
        drawSprite();

    SDL_RenderCopy(renderer_, sprite_, &spriteSize_, &dest_);

    if (selectedHexagone_.has_value()) {
        std::pair<int,int> hexagone = selectedHexagone_.value();
        auto [x, y] = HexagonUtils::axialToPixel(hexagone.first, hexagone.second, hexagonRadius_);
        x += dest_.x + innerHexagonRadius_;
        y += dest_.y + hexagonRadius_;

        // Dessiner un contour épais en jaune
        drawNgon(YELLOW, 6, hexagonRadius_, {x, y}, 3);
    }
}

void GameMap::selectHexagone() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int relX = mouseX - dest_.x - innerHexagonRadius_;
    int relY = mouseY - dest_.y - hexagonRadius_;
    auto [q, r] = HexagonUtils::pixelToAxial(relX, relY, hexagonRadius_);
    auto [x, y] = HexagonUtils::axialToOffset(q, r);
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
        selectedHexagone_.reset();
    else
        selectedHexagone_ = {q, r};
}

void GameMap::handleEvent(SDL_Event &event) {
    if (event.type == SDL_MOUSEMOTION) {
        selectHexagone();
    } else if (event.type == SDL_MOUSEWHEEL) {
        selectHexagone();
    }
}
