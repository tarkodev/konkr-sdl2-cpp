#include "Widgets/GameMap.hpp"
#include "Utils/ColorUtils.hpp"
#include "Utils/HexagonUtils.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_image.h"
#include "Cells/Water.hpp"
#include "Cells/Grounds/Ground.hpp"
#include "Cells/Grounds/PlayableGround.hpp"
#include "Cells/Grounds/Forest.hpp"
#include "GameElements/GameElement.hpp"
#include "SDLWrappers/Coords/Rect.hpp"
#include "SDLWrappers/Coords/Point.hpp"
#include "GameElements/Troops/Bandit.hpp"
#include "GameElements/Town.hpp"
#include "GameElements/Castle.hpp"
#include "GameElements/Camp.hpp"
#include "GameElements/Troops/Villager.hpp"
#include "GameElements/Troops/Pikeman.hpp"
#include "GameElements/Troops/Knight.hpp"
#include "GameElements/Troops/Hero.hpp"
#include "SDLWrappers/Font.hpp"
#include "Displayers/TreasuryDisplayer.hpp"
#include "SDLWrappers/Cursor.hpp"
#include "Utils/Checker.hpp"

#include <stdexcept>
#include <ranges>
#include <random>
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

std::mt19937 GameMap::gen_{};

void GameMap::init() {
    // Create random seed
    gen_ = std::mt19937{ std::random_device{}() };
}

GameMap::GameMap(const Point& pos, const Size size, const std::string mapFile)
  : GameMap(pos, size, getSizeOfMapFile(mapFile), mapFile)
{}

GameMap::GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize, const std::string mapFile)
    : Displayer(pos, size), HexagonGrid<std::shared_ptr<Cell>>(gridSize, nullptr)
{
    if (getWidth() < 2 || getHeight() < 2)
        throw std::runtime_error("Une map doit au moins être de taille 2x2.");

    loadMap(mapFile);
    createCalcs();
    initGame();
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

std::shared_ptr<Cell> GameMap::createCell(char letter, Point pos) {
    switch (letter) {
        case 'F': return std::make_shared<Forest>(pos);
        case 'W': return std::make_shared<Water>();
        default: if (std::isdigit(letter) || std::islower(letter)) return std::make_shared<PlayableGround>(pos);
    }

    throw std::runtime_error(std::string("Caractère inattendu: ") + letter);
}

std::shared_ptr<GameElement> GameMap::createGameElement(char letter, Point pos) {
    switch (letter) {
        case 'B': return std::make_shared<Bandit>(pos);
        case 'T': return std::make_shared<Town>(pos);
        case 'C': return std::make_shared<Castle>(pos);
        case 'A': return std::make_shared<Camp>(pos);
        case 'V': return std::make_shared<Villager>(pos);
        case 'P': return std::make_shared<Pikeman>(pos);
        case 'K': return std::make_shared<Knight>(pos);
        case 'H': return std::make_shared<Hero>(pos);
        case '.': return std::shared_ptr<GameElement>(nullptr);
        default: if (std::islower(letter)) return std::make_shared<Town>(pos, letter - 'a' + 1);
    }

    throw std::runtime_error(std::string("Caractère inattendu: ") + letter);
}

void GameMap::loadMap(const std::string& mapFile) {
    std::ifstream in(mapFile);
    if (!in) throw std::runtime_error("Impossible d'ouvrir le fichier de map.");

    // Get utils dimensions
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();
    std::map<int, std::shared_ptr<Player>> players;

    // Read file
    int y;
    std::string line;
    for (y = 0; y < getHeight() && std::getline(in, line); y++) {
        if (line.empty()) continue;

        int x;
        std::string token;
        std::istringstream iss(line);
        for (x = 0; x < getWidth() && iss >> token; x++) {
            if (token.size() != 2) throw std::runtime_error("Malformation du fichier.");

            // Calculate pos of cell
            auto [posX, posY] = HexagonUtils::offsetToPixel(x, y, islandRadius);
            Point pos{static_cast<int>(posX + islandInnerRadius), static_cast<int>(posY + islandRadius)};

            // Check Cell char
            char cellType = token[0];
            std::shared_ptr<Cell> cell = GameMap::createCell(cellType, pos);

            // if player cell: create and link owner
            if (std::isdigit(cellType) && cellType != '0') {
                int playerId = cellType - '0';
                if (players.find(playerId) == players.end())
                    players[playerId] = std::make_shared<Player>(ColorUtils::getGroundColor(playerId));
                PlayableGround::cast(cell)->setOwner(players[playerId]);
            }
            set(x, y, cell);


            // Don't check if it isn't PlayableGround
            auto pg = PlayableGround::cast(cell);
            if (!pg) continue;


            // Check GameElement char
            char gameEltType = token[1];
            std::shared_ptr<GameElement> gameElt = createGameElement(gameEltType, pos);

            // Set element on cell
            if (std::islower(cellType)) pg->setElement(std::make_shared<Camp>(pos, cellType - 'a' + 1));
            else if (gameEltType != '.' && (cellType != '0' || gameEltType == 'B' || gameEltType == 'A')) {
                pg->setElement(gameElt);
                if (std::islower(gameEltType) || gameEltType == 'T')
                    pg->getOwner()->addTownCell(pg);
            }
        }
        
        for (; x < getWidth(); x++)
            set(x, y, std::make_shared<Water>());
    }

    if (y != getHeight())
        throw std::runtime_error("Le fichier de map n'a pas assez de lignes (attendu " + std::to_string(getHeight()) + ")");

    // Move players
    players_.reserve(players.size());
    for (const auto& [id, playerPtr] : players)
        players_.push_back(playerPtr);

    // update neighbors of cells
    updateNeighbors();
}

void GameMap::saveMap() const {
    const std::string& filename = "../assets/map/Create map.ascii";
    std::ofstream out{filename};
    if (!out)
        throw std::runtime_error("Impossible d'ouvrir le fichier en écriture: " + filename);

    int w = getWidth();
    int h = getHeight();
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            auto cell = get(x, y);
            char cellChar = '.';

            // Cells
            if (Forest::is(cell))       cellChar = 'F';
            else if (Water::is(cell))   cellChar = 'W';
            else if (auto pg = PlayableGround::cast(cell)) {
                if (auto owner = pg->getOwner()) {
                    int id = owner->getNum();
                    cellChar = char('0' + id);
                } else {
                    cellChar = '0';
                }
            }

            // GameElements
            char eltChar = '.';
            if (auto pg = PlayableGround::cast(cell)) {
                auto elt = pg->getElement();
                if (Town::is(elt))          eltChar = 'T';
                else if (Castle::is(elt))   eltChar = 'C';
                else if (Camp::is(elt))     eltChar = 'A';
                else if (Villager::is(elt)) eltChar = 'V';
                else if (Pikeman::is(elt))  eltChar = 'P';
                else if (Knight::is(elt))   eltChar = 'K';
                else if (Hero::is(elt))     eltChar = 'H';
                else if (Bandit::is(elt))   eltChar = 'B';
            }

            // Write tokens
            out << cellChar << eltChar;

            // Write space
            if (x + 1 < w) out << ' ';
        }
        out << '\n';
    }

    // flush et close automatiques à la destruction de ofstream
}

void GameMap::defrayBandits(const std::weak_ptr<Player>& player) {
    auto lplayer = player.lock();
    if (!lplayer) return;
    std::vector<std::weak_ptr<Camp>> camps;
    int nbBandits = 0;

    // Get camps and count bandits
    for (auto& cell : *this) {
        auto pg = PlayableGround::cast(cell);
        if (!pg) continue;

        if (auto camp = Camp::cast(pg->getElement()))
            camps.push_back(camp);
        else if (pg->getOwner() == lplayer && Bandit::is(pg->getElement()))
            nbBandits++;
    }
    
    // Pay bandits
    if (camps.size()) {
        std::uniform_int_distribution<> dist(0, static_cast<int>(camps.size()) - 1);

        // Add coins in camps
        for (int i = 0; i < nbBandits; i++)
            if (auto lcamp = camps[dist(gen_)].lock())
                lcamp->addCoins(1);
    }
}


void GameMap::setPos(const Point& pos) {
    pos_ = pos;
    updateSelectedCell();
}


void GameMap::updateNeighbors() {
    auto w = getWidth();
    auto h = getHeight();

    for (int y = 0; y < h; y++) {
        // Odd cell
        if (y & 1) {
            for (int x = 0; x < w; x++) {
                get(x, y)->setNeighbors({
                    y > 0              ? get(x,   y-1) : nullptr,
                    x > 0              ? get(x-1, y)   : nullptr,
                    y+1 < h            ? get(x,   y+1) : nullptr,
                    x+1 < w && y+1 < h ? get(x+1, y+1) : nullptr,
                    x+1 < w            ? get(x+1, y)   : nullptr,
                    x+1 < w && y > 0   ? get(x+1, y-1) : nullptr,
                });
            }
        }

        // Event cell
        else {
            for (int x = 0; x < w; x++) {
                get(x, y)->setNeighbors({
                    x > 0 && y > 0     ? get(x-1, y-1) : nullptr,
                    x > 0              ? get(x-1, y)   : nullptr,
                    x > 0 && y+1 < h   ? get(x-1, y+1) : nullptr,
                    y+1 < h            ? get(x,   y+1) : nullptr,
                    x+1 < w            ? get(x+1, y)   : nullptr,
                    y > 0              ? get(x,   y-1) : nullptr,
                });
            }
        }
    }
}

const bool GameMap::hasTroopSelected() {
    return selectedTroop_ || boughtElt_;
}


std::weak_ptr<Cell> GameMap::getSelectedCell() const {
    return Rect{0, 0, getWidth(), getHeight()}.contains(selectedCellPos_) ? get(selectedCellPos_.getX(), selectedCellPos_.getY()) : nullptr;
}

void GameMap::setInSelectedCell(const std::shared_ptr<Cell>& cell) {
    if (!(Rect{0, 0, getWidth(), getHeight()}.contains(selectedCellPos_)))
        return;

    if (auto pg = Ground::cast(cell)) {
        // Calculate pos of cell
        double islandInnerRadius = Ground::getInnerRadius();
        double islandRadius = Ground::getRadius();
        auto [posX, posY] = HexagonUtils::offsetToPixel(selectedCellPos_.getX(), selectedCellPos_.getY(), islandRadius);
        Point pos{static_cast<int>(posX + islandInnerRadius), static_cast<int>(posY + islandRadius)};

        pg->setPos(pos);
    }

    set(selectedCellPos_.getX(), selectedCellPos_.getY(), cell);
    updateNeighbors();
}

const int GameMap::getWidth() const {
    return HexagonGrid::getWidth();
}
const int GameMap::getHeight() const {
    return HexagonGrid::getHeight();
}

void GameMap::addWidth(int delta) {
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();
    HexagonGrid::addWidth(delta);
    if (delta > 0) {
        auto w = getWidth();
        auto h = getHeight();
        for (int y = 0; y < h; y++) {
            // Calculate pos of cell
            auto [posX, posY] = HexagonUtils::offsetToPixel(w-1, y, islandRadius);
            Point pos{static_cast<int>(posX + islandInnerRadius), static_cast<int>(posY + islandRadius)};
            
            set(w-1, y, std::make_shared<PlayableGround>(pos));
        }
    }

    updateNeighbors();
    createCalcs();
}

void GameMap::addHeight(int delta) {
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();
    HexagonGrid::addHeight(delta);

    if (delta > 0) {
        auto w = getWidth();
        auto h = getHeight();

        for (int x = 0; x < w; x++) {
            // Calculate pos of cell
            auto [posX, posY] = HexagonUtils::offsetToPixel(x, h-1, islandRadius);
            Point pos{static_cast<int>(posX + islandInnerRadius), static_cast<int>(posY + islandRadius)};
            
            set(x, h-1, std::make_shared<PlayableGround>(pos));
        }
    }

    updateNeighbors();
    createCalcs();
}


void GameMap::createCalcs() {
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
    calcSize_ = {static_cast<int>(cx), static_cast<int>(cy)};
    setProportionalSize(size_);

    // Create calcs
    calc_ = std::make_shared<Texture>(renderer_, calcSize_);
}


void GameMap::refreshIslands() const {
    // Draw islands
    for (auto& cell : *this)
        if (auto g = Ground::cast(cell))
            g->Ground::display(calc_);
}

void GameMap::refreshPlates() const {
    // Draw plates
    for (auto& cell : *this)
        if (auto ground = Ground::cast(cell))
            ground->display(calc_);
}

void GameMap::refreshSelectables() const {
    auto lselectedCell = selectedCell_.lock();
    // Draw selectables
    for (auto& cell : *this) {
        if (auto pg = PlayableGround::cast(cell)) {
            pg->displaySelectable(calc_, lselectedCell && cell == lselectedCell);
        }
    }
}

void GameMap::refreshFences() const {
    // Draw fences
    for (auto& cell : *this)
        if (auto pg = PlayableGround::cast(cell))
            pg->displayFences(calc_);
}

void GameMap::refreshElements() const {
    auto lselectedCell = selectedCell_.lock();
    bool drawCross = lselectedCell && (selectedTroop_ || boughtElt_);

    // Draw game elements
    for (auto& cell : *this) {
        if (auto pg = PlayableGround::cast(cell)) {
            pg->displayElement(calc_);
            if (!drawCross || pg != lselectedCell || !pg->isSelectable())
                pg->displayShield(calc_);
            else
                pg->displayCross(calc_);
        }
    }

    // draw treasury of town
    auto ltownToShowTreasury_ = townToShowTreasury_.lock();
    if (ltownToShowTreasury_)
        ltownToShowTreasury_->displayTreasury(calc_);

    // draw treasury of camp
    auto lcampToShowTreasury_ = campToShowTreasury_.lock();
    if (lcampToShowTreasury_)
        lcampToShowTreasury_->displayTreasury(calc_);
}

void GameMap::refresh() const {
    // Create calcs of map if isn't exists
    if (!calc_) return;

    // Draw transparent background
    calc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    refreshIslands();
    refreshPlates();
    refreshSelectables();
    refreshFences();
    refreshElements();
}


void GameMap::setProportionalSize(const Size size) {
    // Get ratio
    double ratioW = static_cast<double>(size.getWidth()) / calcSize_.getWidth();
    double ratioH = static_cast<double>(size.getHeight()) / calcSize_.getHeight();
    ratio_ = std::min(ratioW, ratioH);

    // Calculate proportionnal destination
    size_ = calcSize_ * ratio_;
}

void GameMap::updateLostElements() {
    for (auto& cell : *this) {
        auto pg = PlayableGround::cast(cell);
        if (!pg) continue;

        // Pass if no element
        auto elt = pg->getElement();
        if (!elt || !elt->isLost()) continue;

        // Transform Castles and troops to camps and bandits
        if (Castle::is(elt))
            pg->setElement(std::make_shared<Camp>(pg->getPos()));
        else if (Troop::is(elt))
            pg->setElement(std::make_shared<Bandit>(pg->getPos()));
    }
}

void GameMap::updateLinks() {
    // Update lost cells
    for (auto& cell : *this)
        if (auto pg = PlayableGround::cast(cell))
            pg->updateLinked();

    // Update lost elements
    updateLostElements();
}

void GameMap::searchNextPlayer() {
    auto cp = currentPlayer_.lock();

    // Search player still in game
    while (!cp || !cp->hasTowns()) {
        players_.erase(players_.begin() + selectedPlayerNum_);
        if (players_.empty()) {
            currentPlayer_.reset();
            return;
        }

        // next player
        selectedPlayerNum_ %= players_.size();
        currentPlayer_ = players_[selectedPlayerNum_];
        cp = currentPlayer_.lock();
    }
}

const bool GameMap::gameFinished() const {
    return gameFinished_;
}

void GameMap::initGame() {
    // Any player found
    if (players_.empty()) {
        refresh();
        return;
    }
    updateLinks();

    // Set the current player
    currentPlayer_ = players_[selectedPlayerNum_];
    searchNextPlayer();

    // Any player has Town
    auto cp = currentPlayer_.lock();
    if (!cp) {
        refresh();
        return;
    }

    // Update next income of players
    for (auto& player : players_)
        if (auto lplayer = player.lock())
            updateIncomes(player);

    // Start turn of current player
    startTurn(currentPlayer_);
    refresh();
}

void GameMap::nextPlayer() {
    auto cp = currentPlayer_.lock();
    if (!cp || players_.empty()) {
        gameFinished_ = true;
        return;
    }

    // Finish turn of current player
    updateLostElements();
    cp->onTurnEnd();
    auto lastCp = cp;

    // Set the new current player
    selectedPlayerNum_ = (selectedPlayerNum_ + 1) % players_.size();
    currentPlayer_ = players_[selectedPlayerNum_];
    searchNextPlayer();

    // Check new current player
    cp = currentPlayer_.lock();
    if (!cp) {
        gameFinished_ = true;
        return;
    } else if (cp == lastCp) {
        gameFinished_ = true;
        return;
    }

    // If first player
    if (selectedPlayerNum_ == 0) {
        moveBandits();
        for (auto& player : players_)
            updateIncomes(player);
    }

    // Start turn of new current player
    startTurn(currentPlayer_);
}

void GameMap::startTurn(std::weak_ptr<Player>& player) {
    if (auto lplayer = player.lock()) { 
        lplayer->onTurnStart();
        defrayBandits(player);
        checkDeficits(player);
        updateFreeTroops(player);
        updateIncomes(player);

        nbUndos_ = 0;
        saves_.clear();
        movedTroops_.clear();
        movedTroopsSave_.clear();
        updateMovables();
    }
}

std::unordered_map<std::shared_ptr<PlayableGround>, int> GameMap::getTreasuresOfCurrentPlayers() {
    std::unordered_map<std::shared_ptr<PlayableGround>, int> towns;
    std::unordered_set<std::shared_ptr<Town>> visited;
    auto cp = currentPlayer_.lock();
    if (!cp) return towns;

    for (auto& cell : cp->getTownCells()) {
        auto pg = PlayableGround::cast(cell);
        if (!pg) continue;

        auto t = Town::cast(pg->getElement());
        if (!t || visited.find(t) != visited.end())
            continue;

        // Calculate cum of treasuries
        int treasury = 0;
        for (auto& town : pg->getTowns()) {
            if (auto ltown = town.lock()) {
                treasury += ltown->getTreasury();
                visited.insert(ltown);
            }
        }
        
        towns[pg] = treasury;
    }

    return towns;
}

const int GameMap::getMaxTreasuryOfCurrentPlayer() {
    auto treasures = getTreasuresOfCurrentPlayers();
    if (treasures.empty()) return 0;

    int max = std::numeric_limits<int>::min();
    for (const auto& [townCell, treasury] : treasures)
        max = std::max(max, treasury);

    return max;
}

void GameMap::buyTroop(const std::shared_ptr<GameElement>& elt) {
    auto cp = currentPlayer_.lock();
    if (!cp || !elt) return;

    // Set new troop
    boughtElt_ = elt;
    elt->setPos(Cursor::getPos());

    // Create temp cell
    selectedNewTroopCell_ = std::make_shared<PlayableGround>(Point{0, 0});
    selectedNewTroopCell_->setOwner(cp);

    // Search potential towns
    for (auto& [townCell, treasury] : getTreasuresOfCurrentPlayers()) {
        if (townCell && treasury >= elt->getCost()) {
            potentialTownCells_.insert(townCell);
            townCell->updateSelectable(Troop::is(elt) ? elt->getStrength() : 0);
        }
    }
}

void GameMap::checkDeficits(std::weak_ptr<Player>& player) {
    auto lplayer = player.lock();
    if (!lplayer) return;

    // Search town in deficit
    int treasury = 0;
    for (auto& townCell : lplayer->getTownCells()) {
        auto ltownCell = townCell.lock();
        if (!ltownCell) continue;
        auto town = Town::cast(ltownCell->getElement());
        if (!town) continue;

        // Pass if no deficit
        treasury = town->getTreasury();
        if (treasury >= 0) continue;

        // Search neighbors towns
        town->setTreasury(0);
        for (auto& neighborTown : ltownCell->getTowns()) {
            auto lneighborTown = neighborTown.lock();
            if (!lneighborTown || lneighborTown == town) continue;

            // Share deficit
            int neighborTreasury = lneighborTown->getTreasury();
            if (neighborTreasury <= 0) {
                continue;
            } else if (neighborTreasury > -treasury) {
                lneighborTown->setTreasury(neighborTreasury + treasury);
                treasury = 0;
                break;
            } else {
                treasury += neighborTreasury;
                lneighborTown->setTreasury(0);
            }
        }

        // Too much deficit
        if (treasury < 0)
            ltownCell->freeTroops();
    }
}

void GameMap::selectCell(const Point& pos) {
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();
    
    // Calculate coords
    Point relPos = pos / ratio_ - Point{static_cast<int>(islandInnerRadius), static_cast<int>(islandRadius)};
    auto [x, y] = HexagonUtils::pixelToOffset(relPos.getX(), relPos.getY(), islandRadius);
    Rect bounds{0, 0, getWidth(), getHeight()};
    Point coords{x, y};

    // Out of bounds
    if (!bounds.contains(coords)) {
        selectedCell_.reset();
        return;
    }
    
    // Set selected cell
    selectedCellPos_ = coords;
    if (auto pg = PlayableGround::cast(get(x, y)))
        selectedCell_ = pg;

    // remove selected cell
    else selectedCell_.reset();
}

void GameMap::updateSelectedCell() {
    selectCell(Cursor::getPos() - pos_);
}

void GameMap::checkWin() {
    int nbPlayerStillTheGame = 0;

    for (auto player : players_)
        if (auto lplayer = player.lock())
            if (lplayer->getTownCells().size())
                nbPlayerStillTheGame++;

    gameFinished_ = nbPlayerStillTheGame < 2;
}

const bool GameMap::placeCastle(const std::weak_ptr<Castle>& castle, const std::weak_ptr<PlayableGround>& to) {
    auto lcastle = castle.lock();
    auto lto = to.lock();
    auto cp = currentPlayer_.lock();
    if (!lcastle || !lto || !cp || lto->getOwner() != cp || lto->getElement())
        return false;

    // Save for undo
    save();

    lto->setElement(lcastle);
    updateIncomes(cp);
    return true;
}

void GameMap::moveTroop(const std::weak_ptr<PlayableGround>& from, const std::weak_ptr<PlayableGround>& to) {
    auto lfrom = from.lock();
    auto lto = to.lock();
    if (!lfrom || !lto || lfrom == lto || !lto->isSelectable()) return;

    // Get interesting elements
    auto fromTroop = Troop::cast(lfrom->getElement());
    if (!fromTroop) return;
    
    auto toTroop = Troop::cast(lto->getElement());
    auto fromOwner = lfrom->getOwner();
    if (!fromOwner) return;
    auto toOwner = lto->getOwner();


    // Different owner
    if (fromOwner != toOwner || Bandit::cast(toTroop)) {
        save();

        // Check destroyed element
        auto destroyTown = Town::cast(lto->getElement());
        auto destroyCamp = Camp::cast(lto->getElement());

        // Move back if troop is back of fences
        if (toTroop && !Bandit::is(toTroop) && lto->hasFences())
            for (auto& nc : lto->getNeighbors())
                if (auto ng = PlayableGround::cast(nc))
                    if (ng->getOwner() == toOwner && !ng->getElement() && ng->hasFences())
                        ng->setElement(lto->getElement());

        lfrom->setElement(nullptr);
        lto->setElement(fromTroop);

        movedTroops_.push_back(fromTroop);
        updateMovables();

        // Give grounds to the new owner
        lto->link(fromOwner);

        // If town has destroyed
        if (destroyTown) {
            if (auto receptionTown = lto->getNearestTown()) {
                receptionTown->setTreasury(receptionTown->getTreasury() + destroyTown->getTreasury());
                checkWin();
            }
        } else if (destroyCamp) {
            if (auto receptionTown = lto->getNearestTown())
                receptionTown->setTreasury(receptionTown->getTreasury() + destroyCamp->getTreasury());
        }

        // Calculate new income
        updateIncomes(fromOwner);
        if (toOwner) updateIncomes(toOwner);
    }
    
    // Same Owner
    else {
        if (!lto->getElement()) {
            save();
            lfrom->setElement(nullptr);
            lto->setElement(fromTroop);
            updateIncomes(fromOwner);
        }

        else if (toTroop) {
            // Merge troops
            std::shared_ptr<Troop> troop = nullptr;
            if (Villager::cast(fromTroop) && Villager::cast(toTroop))
                troop = std::make_shared<Pikeman>(lto->getPos());
            else if (Pikeman::cast(fromTroop) && Pikeman::cast(toTroop))
                troop = std::make_shared<Knight>(lto->getPos());
            else if (Knight::cast(fromTroop) && Knight::cast(toTroop)) 
                troop = std::make_shared<Hero>(lto->getPos());

            if (troop) {
                save();
                
                // Set merged troop
                if (isMovedTroop(fromTroop) || isMovedTroop(toTroop)) {
                    movedTroops_.push_back(troop);
                    updateMovables();
                } else troop->setMovable(true);

                lfrom->setElement(nullptr);
                lto->setElement(troop);

                updateIncomes(fromOwner);
            }
        }
    }
}

void GameMap::moveBandit(const std::weak_ptr<PlayableGround>& from, const std::weak_ptr<PlayableGround>& to) {
    auto lfrom = from.lock();
    auto lto = to.lock();
    if (!lfrom || !lto || lfrom == lto) return;

    // Get interesting elements
    auto fromBandit = Bandit::cast(lfrom->getElement());
    if (!fromBandit || lto->getElement()) return;
    
    // Move bandit
    lfrom->setElement(nullptr);
    lto->setElement(fromBandit);
}

void GameMap::moveBandits() {
    std::unordered_set<std::shared_ptr<Bandit>> movedBandits;
    std::vector<std::shared_ptr<PlayableGround>> playableGrounds;
    playableGrounds.reserve(getWidth() * getHeight());
    bool haveCamp = false;
    bool haveBandit = false;

    for (auto& cell : *this) {
        auto pg = PlayableGround::cast(cell);
        if (!pg) continue;

        // Check if we have camp on the map
        if (!pg->getOwner() && !pg->getElement())
            playableGrounds.push_back(pg);
        haveCamp = haveCamp || Camp::is(pg->getElement());

        auto bandit = Bandit::cast(pg->getElement());
        if (!bandit || movedBandits.find(bandit) != movedBandits.end()) continue;
        haveBandit = true;

        // Get free neighbors
        std::vector<std::weak_ptr<PlayableGround>> candidates;
        for (auto& cell : pg->getNeighbors())
            if (auto ground = PlayableGround::cast(cell))
                if (!ground->getElement())
                    candidates.push_back(ground);

        // Random move bandit
        if (!candidates.empty()) {
            std::uniform_int_distribution<> dist(0, static_cast<int>(candidates.size()) - 1);
            auto dest = candidates[dist(gen_)];

            moveBandit(pg, dest);
            movedBandits.insert(bandit);
        }
    }

    if (!haveCamp && haveBandit && !playableGrounds.empty()) {
        std::uniform_int_distribution<> dist(0, static_cast<int>(playableGrounds.size()) - 1);
        playableGrounds[dist(gen_)]->setElement(std::make_shared<Camp>(Point{0, 0}));
    }
}

const bool GameMap::isMovedTroop(const std::weak_ptr<Troop>& troop) {
    auto ltroop = troop.lock();
    if (!ltroop) return false;
    
    auto it = std::find_if(movedTroops_.begin(), movedTroops_.end(),
        [&](const auto& t) {
            auto locked = t.lock();
            return locked && locked == ltroop;
        });
    
    return (it != movedTroops_.end());
}

const bool GameMap::isSelectableTroop(const std::weak_ptr<PlayableGround>& pgCell) {
    auto pg = pgCell.lock();
    if (!pg) return false;

    // Check owner
    auto owner = pg->getOwner();
    if (!(owner && owner->hasSelected())) return false;

    // Check troop
    auto sTroop = Troop::cast(pg->getElement());
    if (!sTroop || Bandit::is(sTroop) || isMovedTroop(sTroop))
        return false;

    return true;
}

void GameMap::updateCursor() {
    auto lselectedCell = selectedCell_.lock();
    if (lselectedCell && isSelectableTroop(lselectedCell))
        Cursor::requestHand();
    else
        Cursor::requestArrow();
}

void GameMap::onMouseButtonDown(SDL_Event& event) {
    Point mousePos{event.motion.x, event.motion.y};
    potentialTownCells_.clear();
    selectedNewTroopCell_.reset();
    selectedTroopCell_.reset();
    selectedTroop_.reset();
    boughtElt_.reset();

    // Check selected cell
    auto lselectedCell = selectedCell_.lock();
    if (!lselectedCell || !PlayableGround::is(lselectedCell)) return;
    
    // If cell is selectable
    if (isSelectableTroop(lselectedCell)) {
        // Select element
        selectedTroopCell_ = lselectedCell;
        selectedTroop_ = Troop::cast(lselectedCell->getElement());

        if (selectedTroop_) {
            selectedTroop_->setPos(mousePos);
            selectedTroop_->setMovable(false);

            lselectedCell->updateSelectable(selectedTroop_->getStrength());
            lselectedCell->setElement(nullptr);
        }
    }
    
    // If Town is pressed, buy villager
    else {
        auto town = Town::cast(lselectedCell->getElement());
        auto lcurrentPlayer = currentPlayer_.lock();
        if (!town || !lcurrentPlayer || lselectedCell->getOwner() != lcurrentPlayer) 
            return;

        // Get treasury
        int treasury = 0;
        for (auto& town : lselectedCell->getTowns())
            if (auto ltown = town.lock())
                treasury += ltown->getTreasury();
        
        // Check treasury
        auto v = std::make_shared<Villager>(lselectedCell->getPos());
        if (treasury < v->getCost()) return;

        // Set town cell
        selectedTroopCell_ = lselectedCell;

        // Create temp cell
        selectedNewTroopCell_ = std::make_shared<PlayableGround>(Point{0, 0});
        selectedNewTroopCell_->setOwner(lcurrentPlayer);

        // Set new troop
        selectedTroop_ = v;
        v->setPos(mousePos);
        lselectedCell->updateSelectable(v->getStrength());
    }
}

void GameMap::onMouseMotion(SDL_Event& event) {
    Point mousePos{event.motion.x, event.motion.y};
    selectCell(mousePos - pos_);
    townToShowTreasury_.reset();
    campToShowTreasury_.reset();

    // Move selected troop
    if (selectedTroop_) {
        selectedTroop_->setPos(mousePos);
        return;
    } else if (boughtElt_) {
        boughtElt_->setPos(mousePos);
        return;
    }

    // Check hover of elements
    updateCursor();
    if (auto lselectedCell = selectedCell_.lock()) {
        if (auto town = Town::cast(lselectedCell->getElement()))
            townToShowTreasury_ = town;
        else if (auto camp = Camp::cast(lselectedCell->getElement()))
            campToShowTreasury_ = camp;
    }
}

void GameMap::onMouseButtonUp(SDL_Event& event) {
    auto lselectedCell = selectedCell_.lock();
    auto lselectedTroopCell = selectedTroopCell_.lock();

    // Buy by shop
    if (boughtElt_) {
        if (selectedNewTroopCell_ && lselectedCell && PlayableGround::is(lselectedCell)) {
            selectedNewTroopCell_->setElement(boughtElt_);
            int cost = boughtElt_->getCost();

            // Move troop
            if (auto castle = Castle::cast(boughtElt_)) {
                if (placeCastle(castle, selectedCell_))
                    selectedNewTroopCell_->setElement(nullptr);
            } else if (auto troop = Troop::cast(boughtElt_)) {
                troop->setMovable(true);
                moveTroop(selectedNewTroopCell_, selectedCell_);
            }

            // Share purchase
            if (!selectedNewTroopCell_->getElement()) {
                for (auto& town : lselectedCell->getTowns()) {
                    if (auto ltown = town.lock()) {
                        int treasury = ltown->getTreasury();
                        if (treasury > cost) {
                            ltown->setTreasury(treasury - cost);
                            break;
                        }

                        cost -= treasury;
                        ltown->setTreasury(0);
                    }
                }
                if (auto cp = currentPlayer_.lock())
                    updateIncomes(cp);
            }
        }
        
        // Remove possibilities
        for (auto& townCell : potentialTownCells_)
            townCell->updateSelectable(-1);
    }

    // Buy by Town / Move troop
    else if (lselectedTroopCell && selectedTroop_) {

        // Buy Troop
        if (selectedNewTroopCell_) {
            if (lselectedCell && PlayableGround::is(lselectedCell)) {
                selectedNewTroopCell_->setElement(selectedTroop_);
                int cost = selectedTroop_->getCost();

                // Move troop
                selectedTroop_->setMovable(true);
                moveTroop(selectedNewTroopCell_, selectedCell_);

                // Share purchase
                if (!selectedNewTroopCell_->getElement()) {
                    for (auto& town : lselectedCell->getTowns()) {
                        if (auto ltown = town.lock()) {
                            int treasury = ltown->getTreasury();
                            if (treasury > cost) {
                                ltown->setTreasury(treasury - cost);
                                break;
                            }

                            cost -= treasury;
                            ltown->setTreasury(0);
                        }
                    }
                    if (auto cp = currentPlayer_.lock())
                        updateIncomes(cp);
                }
            }

            // Remove possibilities
            lselectedTroopCell->updateSelectable(-1);
        }

        // Move Troop
        else {
            lselectedTroopCell->setElement(selectedTroop_);
            selectedTroop_->setMovable(true);
    
            // Move troop
            if (lselectedCell)
                moveTroop(selectedTroopCell_, selectedCell_);
    
            // Remove possibilities
            lselectedTroopCell->updateSelectable(-1);
        }
    }

    potentialTownCells_.clear();
    selectedNewTroopCell_.reset();
    selectedTroopCell_.reset();
    selectedTroop_.reset();
    boughtElt_.reset();
    updateCursor();
}

void GameMap::handleEvent(SDL_Event &event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: onMouseButtonDown(event); break;
        case SDL_MOUSEMOTION: onMouseMotion(event); break;
        case SDL_MOUSEBUTTONUP: onMouseButtonUp(event); break;
    }
}

void GameMap::display(const std::weak_ptr<BlitTarget>& target) const {
    // Check initialized calcs
    refresh();

    // Check target
    auto ltarget = target.lock();
    if (!ltarget) return;

    // Show map
    Rect dest = {pos_, size_};
    if (calc_) ltarget->blit(calc_, dest);

    // Show selected troop
    if (selectedTroop_) selectedTroop_->display(ltarget);
    else if (boughtElt_) boughtElt_->display(ltarget);
}

void GameMap::updateIncomes(std::weak_ptr<Player>& player) {
    auto lplayer = player.lock();
    if (!lplayer) return;

    // Get treasures and reset incomes
    std::map<std::shared_ptr<Town>, int>towns;
    for (auto& townCell : lplayer->getTownCells()) {
        if (auto ltownCell = townCell.lock()) {
            auto town = Town::cast(ltownCell->getElement());
            towns[town] = town->getTreasury();
            town->setIncome(0);
        }
    }

    // Calculate incomes of towns of player
    for (auto& cell : *this) {
        auto pg = PlayableGround::cast(cell);
        if (!pg || pg->getOwner() != lplayer) continue;
        
        pg->updateIncome();
        auto troop = Troop::cast(pg->getElement());
        if (troop && !Bandit::cast(troop))
            troop->setFree(false);
    }

    // Share deficits
    for (auto& townCell : lplayer->getTownCells()) {
        if (auto ltownCell = townCell.lock()) {
            auto town = Town::cast(ltownCell->getElement());
            town->updateTreasury();
        }
    }
    checkDeficits(player);

    // restore treasures
    for (auto& [town, treasury] : towns)
        town->setTreasury(treasury);
}

void GameMap::updateIncomes(std::shared_ptr<Player>& player) {
    std::weak_ptr<Player> wplayer = player;
    updateIncomes(wplayer);
}

void GameMap::updateMovables() {
    auto cp = currentPlayer_.lock();
    for (auto& cell : *this)
        if (auto pg = PlayableGround::cast(cell))
            if (auto troop = Troop::cast(pg->getElement()))
                troop->setMovable(cp && cp == pg->getOwner() && !Bandit::is(troop) && !isMovedTroop(troop));
}

void GameMap::undo() {
    if (nbUndos_ < 1) return;
    std::pair<int, int> gridSize{getWidth(), getHeight()};
    movedTroops_ = movedTroopsSave_;
    auto oldGrid = saves_.back();
    saves_.pop_back();
    nbUndos_--;

    // Place old cells
    for (int x = 0; x < gridSize.first; x++) {
        for (int y = 0; y < gridSize.second; y++) {
            auto oldCell = oldGrid.get(x, y);

            // Set copied town to players
            if (auto oldPg = PlayableGround::cast(oldCell))
                if (auto town = Town::cast(oldPg->getElement()))
                    if (auto owner = oldPg->getOwner())
                        owner->addTownCell(oldPg);

            // Set the copied cell to the grid
            set(x, y, oldCell);
        }
    }

    // Update neighbors
    updateNeighbors();
    updateMovables();

    // refresh towns of players
    for (auto player : players_)
        if (auto lplayer = player.lock())
            lplayer->updateTowns();
}

void GameMap::save() {
    std::pair<int, int> gridSize{getWidth(), getHeight()};
    HexagonGrid<std::shared_ptr<Cell>> copiedGrid(gridSize, nullptr);

    // Copy all cells
    for (int x = 0; x < gridSize.first; x++) {
        for (int y = 0; y < gridSize.second; y++) {
            // Copy cell
            auto cell = get(x, y);
            auto cellCopy = cell->deepCopy();
            auto pg = PlayableGround::cast(cell);
            copiedGrid.set(x, y, cellCopy);

            // copy moved troops
            if (pg && isMovedTroop(Troop::cast(pg->getElement())))
                movedTroopsSave_.push_back(Troop::cast(PlayableGround::cast(cellCopy)->getElement()));
        }
    }

    saves_.push_back(copiedGrid);
    nbUndos_++;
}

void GameMap::updateFreeTroops(const std::weak_ptr<Player>& player) {
    auto lplayer = player.lock();
    if (!lplayer) return;

    // Search free troops
    for (auto& cell : *this) {
        auto pg = PlayableGround::cast(cell);
        if (!pg || pg->getOwner() != lplayer) continue;

        // Path if no free troop
        auto troop = Troop::cast(pg->getElement());
        if (!troop || Bandit::cast(troop) || !troop->isFree()) continue;

        // Transform to bandit
        pg->setElement(std::make_shared<Bandit>(pg->getPos()));
    }
}
