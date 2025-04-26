#include "GameMap.hpp"
#include "ColorUtils.hpp"
#include "HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Water.hpp"
#include "Ground.hpp"
#include "PlayableGround.hpp"
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
    selectSprite_ = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha(); //!temp
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
                set(x, y, new PlayableGround());
            else if (v == 1)
                set(x, y, new Water());
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
                case 'F': {
                    cell = new Forest();
                    break;
                }

                case 'W': {
                    cell = new Water();
                    break;
                }
                
                case '0': {
                    cell = new PlayableGround();
                    break;
                }

                default: {
                    if (!std::isdigit(cellType))
                        throw std::runtime_error("Caractère inattendu: " + std::to_string(cellType));

                    //! mettre au propre
                    int playerId = cellType - '0';
                    if (players.find(playerId) == players.end())
                        players[playerId] = new Player(std::string("Player ") + std::to_string(playerId), ColorUtils::getGroundColor(playerId));

                    cell = new PlayableGround(players[playerId]);
                    break;
                }
            }

            //! décommenter une fois implémenté
            char gameEltType = token[1];
            GameElement* gameElt = nullptr;
            switch (gameEltType) {
                case 'B': {
                    gameElt = new Bandit();
                    if (cellType != 'W' && cellType != 'F')
                        dynamic_cast<PlayableGround*>(cell)->setElement(gameElt);
                    break;
                }

                case 'T': {
                    gameElt = new Town();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType != '0') t->setElement(gameElt);
                    break;
                }

                case 'C': {
                    gameElt = new Castle();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType != '0') t->setElement(gameElt);
                    break;
                }

                case 'A': {
                    gameElt = new Camp();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType == '0') t->setElement(gameElt);
                    break;
                }

                case 'V': {
                    gameElt = new Villager();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType != '0') t->setElement(gameElt);
                    break;
                }

                case 'P': {
                    gameElt = new Pikeman();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType != '0') t->setElement(gameElt);
                    break;
                }

                case 'K': {
                    gameElt = new Knight();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType != '0') t->setElement(gameElt);
                    break;
                }

                case 'H': {
                    gameElt = new Hero();
                    PlayableGround* t = dynamic_cast<PlayableGround*>(cell);
                    if (t && cellType != '0') t->setElement(gameElt);
                    break;
                }

                default:
                    break;
            }

            set(x, y, cell);
        }
        
        for (; x < getWidth(); x++)
            set(x, y, new Water());
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
    Size islandSize = Ground::getIslandSize();
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();

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

    // Create sprite for elements
    cells_ = new Texture(renderer_, spriteSize_);
    cells_->convertAlpha();


    // Delete elements sprite if already exists
    if (fences_) {
        delete fences_;
        fences_ = nullptr;
    }

    // Create sprite for elements
    fences_ = new Texture(renderer_, spriteSize_);
    fences_->convertAlpha();


    // Delete elements sprite if already exists
    if (elements_) {
        delete elements_;
        elements_ = nullptr;
    }

    // Create sprite for elements
    elements_ = new Texture(renderer_, spriteSize_);
    elements_->convertAlpha();
}

void GameMap::refresh()
{
    // Update Neighbors of cells
    updateNeighbors();

    // Create sprite of map if isn't exists
    if (!islands_ || !cells_ || !fences_ || !elements_)
        createSprite();

    // Draw transparent background
    //! faire un calc général qui est refresh à chaque changement d'un de ces troiq calques et qui est blit directement (cela permettra de diviser par 3 le nombre de blit)
    islands_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    cells_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    fences_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    elements_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    // Get utils dimensions
    Size islandSize = Ground::getIslandSize();
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();

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
            if (auto* t = dynamic_cast<Ground*>(cell))
                t->Ground::display(islands_, pos);

            // Draw cell
            if (auto* disp = dynamic_cast<Displayer*>(cell))
                disp->display(cells_, pos);

            // Draw element
            if (auto* pt = dynamic_cast<PlayableGround*>(cell)) {
                pt->displayFences(fences_, pos);
                if (auto* elt = pt->getElement())
                    elt->display(elements_, pos);
            }
        }
    }
}

void GameMap::setProportionalSize(const Size size) {
    double islandRadius = Ground::getRadius();

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
    SDL_RenderCopy(renderer_, fences_->get(), nullptr, &dest.get());
    SDL_RenderCopy(renderer_, elements_->get(), nullptr, &dest.get());

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

    dynamic_cast<PlayableGround*>(get(selectedHexagon_->getX(), selectedHexagon_->getY()))->setOwner(nullptr);
    //set(selectedHexagon_->getX(), selectedHexagon_->getY(), new Water());
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
