#include "GameMap.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Sea.hpp"
#include "Territory.hpp"
#include "PlayerTerritory.hpp"
#include "Plain.hpp"
#include "Forest.hpp"
#include "logic/GameElement.hpp"
#include "RenderTargetGuard.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "DrawUtils.hpp"
#include "logic/units/Bandit.hpp"
#include "logic/units/Town.hpp"
#include "logic/units/Castle.hpp"
#include "logic/units/Camp.hpp"
#include "logic/units/Villager.hpp"
#include "logic/units/Pikeman.hpp"
#include "logic/units/Knight.hpp"
#include "logic/units/Hero.hpp"

#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>


SDL_Renderer* GameMap::renderer_ = nullptr;
Texture* GameMap::selectSprite_ = nullptr;

void GameMap::init(SDL_Renderer *renderer) {
    renderer_ = renderer;
    selectSprite_ = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha();
}

GameMap::GameMap(const Size size, const std::pair<int, int>& gridSize)
    : size_{size}, HexagonGrid<Cell*>(gridSize, nullptr)
{
    if (getWidth() < 2 || getHeight() < 2)
        throw std::runtime_error("Une map doit au moins être de taille 2x2.");

    for (int x=0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            int v = std::rand() % 4;
            if (v == 0)
                set(x, y, new PlayerTerritory());
            else if (v == 1)
                set(x, y, new Sea());
            else if (v == 2)
                set(x, y, new Plain());
            else if (v == 3)
                set(x, y, new Forest());
        }
    }
    refresh();
}

GameMap::GameMap(const Size size, const std::string mapFile)
  : GameMap(size, getSizeOfMapFile(mapFile), mapFile)
{}

GameMap::GameMap(const Size size, const std::pair<int, int>& gridSize, const std::string mapFile)
    : size_{size}, HexagonGrid<Cell*>(gridSize, nullptr)
{
    if (getWidth() < 2 || getHeight() < 2)
        throw std::runtime_error("Une map doit au moins être de taille 2x2.");

    loadMap(mapFile);
    refresh();
}

std::pair<int,int> GameMap::getSizeOfMapFile(const std::string& mapFile) {
    std::ifstream in(mapFile);
    if (!in) throw std::runtime_error("Impossible d'ouvrir le fichier de map: " + mapFile);

    int width = 0;
    int height;
    std::string line;

    for (height = 0; std::getline(in, line); height++) {
        // Trim
        auto first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        auto last  = line.find_last_not_of(" \t\r\n");
        std::string trimmed = line.substr(first, last - first + 1);

        // Count cells
        int tokens = 0;
        std::string tok;
        std::istringstream iss(trimmed);
        while (iss >> tok) tokens++;

        width = std::max(width, tokens);
    }

    return { width, height };
}

void GameMap::loadMap(const std::string& mapFile) {
    std::ifstream in(mapFile);
    if (!in) throw std::runtime_error("Impossible d'ouvrir le fichier de map.");

    std::map<int, Player*> players;

    int y;
    std::string line;
    for (y = 0; y < getHeight() && std::getline(in, line); y++) {
        if (line.empty()) continue;

        int x;
        std::string token;
        std::istringstream iss(line);
        for (x = 0; x < getWidth() && iss >> token; x++) {
            if (token.size() != 2)
                throw std::runtime_error("Malformation du fichier.");

            char cellType = token[0];
            Cell* cell = nullptr;
            switch (cellType) {
                case 'T': {
                    cell = new PlayerTerritory();
                    break;
                }

                case 'P': {
                    cell = new Plain();
                    break;
                }

                case 'F': {
                    cell = new Forest();
                    break;
                }

                case 'S': {
                    cell = new Sea();
                    break;
                }

                default: {
                    if (!std::isdigit(cellType))
                        throw std::runtime_error("Caractère inattendu: " + std::to_string(cellType));

                    //! mettre au propre
                    int playerId = cellType - '0';
                    if (players.find(playerId) == players.end())
                        players[playerId] = new Player(std::string("Player ") + std::to_string(playerId), std::vector{ColorUtils::INDIAN_RED, ColorUtils::MEDIUM_VIOLET_RED, ColorUtils::DARK_ORANGE, ColorUtils::OLIVE_DRAB, ColorUtils::MEDIUM_AQUAMARINE, ColorUtils::STEEL_BLUE, ColorUtils::PLUM, ColorUtils::SADDLE_BROWN, ColorUtils::SEABLUE}[playerId]);

                    cell = new PlayerTerritory(players[playerId]);
                    break;
                }
            }

            //! décommenter une fois implémenté
            char gameEltType = token[1];
            GameElement* gameElt = nullptr;
            switch (gameEltType) {
                case 'B': {
                    gameElt = new Bandit();
                    if (cellType != 'S' && cellType != 'F')
                        dynamic_cast<PlayableTerritory*>(cell)->setElement(gameElt);
                    break;
                }

                case 'T': {
                    gameElt = new Town();
                    PlayerTerritory* t = dynamic_cast<PlayerTerritory*>(cell);
                    if (t) t->setElement(gameElt);
                    break;
                }

                case 'C': {
                    gameElt = new Castle();
                    PlayerTerritory* t = dynamic_cast<PlayerTerritory*>(cell);
                    if (t) t->setElement(gameElt);
                    break;
                }

                case 'A': {
                    gameElt = new Camp();
                    PlayableTerritory* t = dynamic_cast<PlayableTerritory*>(cell);
                    if (t && cellType == 'T')
                        t->setElement(gameElt);
                    break;
                }

                case 'V': {
                    gameElt = new Villager();
                    PlayerTerritory* t = dynamic_cast<PlayerTerritory*>(cell);
                    if (t) t->setElement(gameElt);
                    break;
                }

                case 'P': {
                    gameElt = new Pikeman();
                    PlayerTerritory* t = dynamic_cast<PlayerTerritory*>(cell);
                    if (t) t->setElement(gameElt);
                    break;
                }

                case 'K': {
                    gameElt = new Knight();
                    PlayerTerritory* t = dynamic_cast<PlayerTerritory*>(cell);
                    if (t) t->setElement(gameElt);
                    break;
                }

                case 'H': {
                    gameElt = new Hero();
                    PlayerTerritory* t = dynamic_cast<PlayerTerritory*>(cell);
                    if (t) t->setElement(gameElt);
                    break;
                }

                default:
                    break;
            }

            set(x, y, cell);
        }
        
        for (; x < getWidth(); x++)
            set(x, y, new Sea());
    }

    if (y != getHeight())
        throw std::runtime_error("Le fichier de map n'a pas assez de lignes (attendu " + std::to_string(getHeight()) + ")");
}

void GameMap::updateNeighbors() {
    for (int y = 0; y < getHeight(); y++) {
        if (y & 1) { // odd line
            for (int x = 0; x < getWidth(); x++) {
                get(x, y)->setNeighbors({
                    y > 0                                 ? get(x,   y-1)   : nullptr,
                    x > 0                                 ? get(x-1, y)   : nullptr,
                    y+1 < getHeight()                     ? get(x,   y+1)   : nullptr,
                    x+1 < getWidth() && y+1 < getHeight() ? get(x+1, y+1) : nullptr,
                    x+1 < getWidth()                      ? get(x+1, y)   : nullptr,
                    x+1 < getWidth() && y > 0             ? get(x+1, y-1) : nullptr,
                });
            }
        } else { // even line
            for (int x = 0; x < getWidth(); x++) {
                get(x, y)->setNeighbors({
                    x > 0 && y > 0                        ? get(x-1, y-1) : nullptr,
                    x > 0                                 ? get(x-1, y)   : nullptr,
                    x > 0 && y+1 < getHeight()            ? get(x-1, y+1) : nullptr,
                    y+1 < getHeight()                     ? get(x,   y+1)   : nullptr,
                    x+1 < getWidth()                      ? get(x+1, y)   : nullptr,
                    y > 0                                 ? get(x,   y-1)   : nullptr,
                });
            }
        }
    }
}

void GameMap::createSprite() {
    // Get utils dimensions
    Size islandSize = Territory::getIslandSize();
    double islandInnerRadius = Territory::getInnerRadius();
    double islandRadius = Territory::getRadius();

    // Get pos of last hexagon
    auto cx = HexagonUtils::offsetToPixel(getWidth()-1, 1, islandRadius).first;
    auto cy = HexagonUtils::offsetToPixel(0, getHeight()-1, islandRadius).second;
    cx += islandInnerRadius + islandSize.getWidth() / 2.0;
    cy += islandRadius + islandSize.getHeight() / 2.0;

    // Resize 
    spriteSize_ = {static_cast<int>(cx), static_cast<int>(cy)};
    setProportionalSize(size_);

    // Delete islands sprite if already exists
    if (islands_) {
        delete islands_;
        islands_ = nullptr;
    }

    // Create sprite for islands
    islands_ = new Texture(renderer_, spriteSize_);
    islands_->convertAlpha();

    // Delete cells sprite if already exists
    if (cells_) {
        delete cells_;
        cells_ = nullptr;
    }

    // Create sprite for cells
    cells_ = new Texture(renderer_, spriteSize_);
    cells_->convertAlpha();
}

void GameMap::refresh()
{
    // Update Neighbors of cells
    updateNeighbors();

    // Create sprite of map if isn't exists
    if (!islands_ || !cells_)
        createSprite();

    // Draw transparent background
    islands_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    cells_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    // Get utils dimensions
    Size islandSize = Territory::getIslandSize();
    double islandInnerRadius = Territory::getInnerRadius();
    double islandRadius = Territory::getRadius();

    // Draw islands and cells
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            Cell* cell = get(x, y);

            // Calcul de la position de blit
            auto [q, r] = HexagonUtils::offsetToAxial(x, y);
            auto [cx, cy] = HexagonUtils::axialToPixel(q, r, islandRadius);
            auto pos = Point{
                static_cast<int>(cx + islandInnerRadius),
                static_cast<int>(cy + islandRadius)
            };

            // Draw island
            if (auto* t = dynamic_cast<Territory*>(cell))
                t->Territory::display(islands_, pos);

            if (auto* disp = dynamic_cast<Displayer*>(cell))
                disp->display(cells_, pos);
        }
    }
}

void GameMap::setProportionalSize(const Size size) {
    double islandRadius = Territory::getRadius();

    // Get ratio
    double ratioW = static_cast<double>(size.getWidth()) / spriteSize_.getWidth();
    double ratioH = static_cast<double>(size.getHeight()) / spriteSize_.getHeight();
    double ratio = std::min(ratioW, ratioH);

    // Calculate proportionnal destination
    size_ = spriteSize_ * ratio;
    cellRadius_ = islandRadius * ratio;
    innerCellRadius_ = std::sqrt(3) * cellRadius_ / 2.0;
}

Size GameMap::getSize() const {
    return size_;
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
        auto [x, y] = HexagonUtils::axialToPixel(q, r, cellRadius_);
        x += pos.getX() + innerCellRadius_;
        y += pos.getY() + cellRadius_;

        double ratio = static_cast<double>(dest.getWidth()) / islands_->getWidth();
        int w = static_cast<int>(ratio * selectSprite_->getWidth());
        int h = static_cast<int>(ratio * selectSprite_->getHeight());

        SDL_RenderCopy(renderer_, selectSprite_->get(), nullptr, new SDL_Rect{static_cast<int>(x -  w/2), static_cast<int>(y - h/2), w, h});
    }
}

void GameMap::test() {
    if (!selectedHexagon_.has_value()) return;

    set(selectedHexagon_->getX(), selectedHexagon_->getY(), new Sea());
    refresh();
}

void GameMap::selectHexagon(const Point& pos) {
    int relX = pos.getX() - innerCellRadius_;
    int relY = pos.getY() - cellRadius_;

    auto [q, r] = HexagonUtils::pixelToAxial(relX, relY, cellRadius_);
    auto [x, y] = HexagonUtils::axialToOffset(q, r);
    
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
        selectedHexagon_.reset();
    else
        selectedHexagon_ = {x, y};
}

void GameMap::handleEvent(SDL_Event &event) {}
