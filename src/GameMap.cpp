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
#include "logic/units/Bandit.hpp"
#include "logic/units/Town.hpp"
#include "logic/units/Castle.hpp"
#include "logic/units/Camp.hpp"
#include "logic/units/Villager.hpp"
#include "logic/units/Pikeman.hpp"
#include "logic/units/Knight.hpp"
#include "logic/units/Hero.hpp"
#include "Font.hpp"
#include "TreasuryDisplayer.hpp"

#include <ranges>
#include <random>
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>


Texture* GameMap::selectSprite_ = nullptr;
SDL_Cursor* GameMap::handCursor_ = nullptr;
SDL_Cursor* GameMap::arrowCursor_ = nullptr;
std::mt19937 GameMap::gen_{};

void GameMap::init() {
    // Load sprites and cursors
    selectSprite_ = (new Texture(renderer_, "../assets/img/plate.png"))->convertAlpha(); //!temp
    handCursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    arrowCursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

    // Create random seed
    gen_ = std::mt19937{ std::random_device{}() };
}


GameMap::GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize)
    : Displayer(pos), size_{size}, HexagonGrid<Cell*>(gridSize, nullptr)
{
    if (getWidth() < 2 || getHeight() < 2)
        throw std::runtime_error("Une map doit au moins être de taille 2x2.");

    double islandRadius = Ground::getRadius();
    double islandInnerRadius = Ground::getInnerRadius();
    for (int x=0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            auto [posX, posY] = HexagonUtils::offsetToPixel(x, y, islandRadius);
            Point pos{static_cast<int>(posX + islandInnerRadius), static_cast<int>(posY + islandRadius)};

            int v = std::rand() % 3;
            if (v == 0)
                set(x, y, new PlayableGround(pos));
            else if (v == 1)
                set(x, y, new Water());
            else if (v == 2)
                set(x, y, new Forest(pos));
        }
    }

    updateNeighbors();
    startGame();
}

GameMap::GameMap(const Point& pos, const Size size, const std::string mapFile)
  : GameMap(pos, size, getSizeOfMapFile(mapFile), mapFile)
{}

GameMap::GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize, const std::string mapFile)
    : Displayer(pos), size_{size}, HexagonGrid<Cell*>(gridSize, nullptr)
{
    if (getWidth() < 2 || getHeight() < 2)
        throw std::runtime_error("Une map doit au moins être de taille 2x2.");

    loadMap(mapFile);
    startGame();
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
    //! Si on a le temps, découper cette fonction (voir la mettre dans une classe à part entière avec getSizeOfMapFile)
    std::ifstream in(mapFile);
    if (!in) throw std::runtime_error("Impossible d'ouvrir le fichier de map.");

    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();
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

            auto [posX, posY] = HexagonUtils::offsetToPixel(x, y, islandRadius);
            Point pos{static_cast<int>(posX + islandInnerRadius), static_cast<int>(posY + islandRadius)};

            char cellType = token[0];
            Cell* cell = nullptr;
            switch (cellType) {
                case 'F': {
                    cell = new Forest(pos);
                    break;
                }

                case 'W': {
                    cell = new Water();
                    break;
                }
                
                case '0': {
                    cell = new PlayableGround(pos);
                    break;
                }

                default: {
                    if (!std::isdigit(cellType))
                        throw std::runtime_error("Caractère inattendu: " + std::to_string(cellType));

                    int playerId = cellType - '0';
                    if (players.find(playerId) == players.end()) {
                        players[playerId] = new Player(std::string("Player ") + std::to_string(playerId), ColorUtils::getGroundColor(playerId));
                        players_.push_back(players[playerId]);
                    }

                    cell = new PlayableGround(pos, players[playerId]);
                    break;
                }
            }

            char gameEltType = token[1];
            GameElement* gameElt = nullptr;
            switch (gameEltType) {
                case 'B': {
                    gameElt = new Bandit(pos);
                    if (cellType != 'W' && cellType != 'F')
                        dynamic_cast<PlayableGround*>(cell)->setElement(gameElt);
                    break;
                }

                case 'T': {
                    gameElt = new Town(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType != '0') {
                        pg->setElement(gameElt);
                        pg->getOwner()->addTownCell(pg);
                    }
                    break;
                }

                case 'C': {
                    gameElt = new Castle(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType != '0') pg->setElement(gameElt);
                    break;
                }

                case 'A': {
                    gameElt = new Camp(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType == '0') pg->setElement(gameElt);
                    break;
                }

                case 'V': {
                    gameElt = new Villager(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType != '0') pg->setElement(gameElt);
                    break;
                }

                case 'P': {
                    gameElt = new Pikeman(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType != '0') pg->setElement(gameElt);
                    break;
                }

                case 'K': {
                    gameElt = new Knight(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType != '0') pg->setElement(gameElt);
                    break;
                }

                case 'H': {
                    gameElt = new Hero(pos);
                    PlayableGround* pg = dynamic_cast<PlayableGround*>(cell);
                    if (pg && cellType != '0') pg->setElement(gameElt);
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

    updateNeighbors();
}

void GameMap::startGame() {
    // Update links
    for (Cell* cell : *this)
        if (auto* pg = dynamic_cast<PlayableGround*>(cell))
            pg->updateLinked();
    updateLostElements();

    currentPlayer_ = players_[selectedPlayerNum_];
    for (Player *player : players_)
        updateIncomes(player);

    currentPlayer_->onTurnStart();
    refresh();
}


void GameMap::setPos(const Point& pos) {
    pos_ = pos;
    updateSelectedCell();
}

const Size GameMap::getSize() const {
    return size_;
}


void GameMap::updateNeighbors() {
    for (int y = 0; y < getHeight(); y++) {
        if (y & 1) {
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
        }
        else {
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

    // Delete islands sprite if already exists
    if (islandsCalc_) {
        delete islandsCalc_;
        islandsCalc_ = nullptr;
    }

    // Create sprite for islands
    islandsCalc_ = new Texture(renderer_, calcSize_);
    islandsCalc_->convertAlpha();


    // Delete cells sprite if already exists
    if (platesCalc_) {
        delete platesCalc_;
        platesCalc_ = nullptr;
    }

    // Create sprite for elements
    platesCalc_ = new Texture(renderer_, calcSize_);
    platesCalc_->convertAlpha();


    // Delete cells sprite if already exists
    if (selectablesCalc_) {
        delete selectablesCalc_;
        selectablesCalc_ = nullptr;
    }

    // Create sprite for elements
    selectablesCalc_ = new Texture(renderer_, calcSize_);
    selectablesCalc_->convertAlpha();


    // Delete elements sprite if already exists
    if (fencesCalc_) {
        delete fencesCalc_;
        fencesCalc_ = nullptr;
    }

    // Create sprite for elements
    fencesCalc_ = new Texture(renderer_, calcSize_);
    fencesCalc_->convertAlpha();


    // Delete elements sprite if already exists
    if (elementsCalc_) {
        delete elementsCalc_;
        elementsCalc_ = nullptr;
    }

    // Create sprite for elements
    elementsCalc_ = new Texture(renderer_, calcSize_);
    elementsCalc_->convertAlpha();


    // Delete elements sprite if already exists
    if (calc_) {
        delete calc_;
        calc_ = nullptr;
    }

    // Create sprite for elements
    calc_ = new Texture(renderer_, calcSize_);
    calc_->convertAlpha();
}


void GameMap::refreshIslands() {
    if (!platesCalc_) refresh();

    // Draw transparent background
    platesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    // Draw islands
    for (Cell* cell : *this)
        if (auto* g = dynamic_cast<Ground*>(cell))
            g->Ground::display(islandsCalc_);

    refreshMain();
}

void GameMap::refreshPlates() {
    if (!platesCalc_) refresh();

    // Draw transparent background
    platesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    // Draw plates
    for (Cell* cell : *this)
        if (auto* disp = dynamic_cast<Displayer*>(cell))
            disp->display(platesCalc_);

    refreshMain();
}

void GameMap::refreshSelectables() {
    if (!selectablesCalc_) refresh();

    // Draw transparent background
    selectablesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    // Draw selectables
    for (Cell* cell : *this)
        if (auto* pg = dynamic_cast<PlayableGround*>(cell))
            pg->displaySelectable(selectablesCalc_);

    refreshMain();
}

void GameMap::refreshFences() {
    if (!fencesCalc_) refresh();

    // Draw transparent background
    fencesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    // Draw fences
    for (Cell* cell : *this)
        if (auto* pg = dynamic_cast<PlayableGround*>(cell))
            pg->displayFences(fencesCalc_);

    refreshMain();
}

void GameMap::refreshElements() {
    if (!elementsCalc_) refresh();

    // Draw transparent background
    elementsCalc_->fill(ColorUtils::toTransparent(ColorUtils::GREY));

    // Draw game elements
    for (Cell* cell : *this) {
        if (auto* pg = dynamic_cast<PlayableGround*>(cell)) {
            pg->displayElement(elementsCalc_);
            pg->displayShield(elementsCalc_);
        }
    }

    if (townToShowTreasury_)
        townToShowTreasury_->displayTreasury(elementsCalc_);

    refreshMain();
}

void GameMap::refreshMain() {
    if (!calc_) refresh();

    calc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));

    calc_->blit(islandsCalc_);
    calc_->blit(platesCalc_);
    calc_->blit(selectablesCalc_);
    calc_->blit(fencesCalc_);
    calc_->blit(elementsCalc_);
}

void GameMap::refresh()
{
    // Create calcs of map if isn't exists
    if (!islandsCalc_ || !platesCalc_ || !selectablesCalc_ || !fencesCalc_ || !elementsCalc_ || !calc_)
        createCalcs();

    // Draw transparent background
    islandsCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    platesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    selectablesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    fencesCalc_->fill(ColorUtils::toTransparent(ColorUtils::SEABLUE));
    elementsCalc_->fill(ColorUtils::toTransparent(ColorUtils::GREY));

    // Draw islands and cells
    for (Cell* cell : *this) {
        // Draw island
        if (auto* g = dynamic_cast<Ground*>(cell))
            g->Ground::display(islandsCalc_);

        // Draw cell
        if (auto* disp = dynamic_cast<Displayer*>(cell))
            disp->display(platesCalc_);

        // Draw element
        if (auto* pg = dynamic_cast<PlayableGround*>(cell)) {
            pg->displaySelectable(selectablesCalc_);
            pg->displayFences(fencesCalc_);
            pg->displayElement(elementsCalc_);
            pg->displayShield(elementsCalc_);
        }
    }

    refreshMain();
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
    for (Cell* cell : *this) {
        auto* pg = dynamic_cast<PlayableGround*>(cell);
        if (!pg) continue;

        auto* elt = pg->getElement();
        if (!elt || !elt->isLost()) continue;

        if (dynamic_cast<Castle*>(elt)) {
            delete elt;
            pg->setElement(new Camp(pg->getPos()));
        } else if (dynamic_cast<Troop*>(elt)) {
            delete elt;
            pg->setElement(new Bandit(pg->getPos()));
        }
    }
}

void GameMap::nextPlayer() {
    if (players_.empty()) return;

    // Finish turn of current player
    updateLostElements();
    currentPlayer_->onTurnEnd();

    // Set the new current player
    selectedPlayerNum_ = (selectedPlayerNum_ + 1) % players_.size();
    currentPlayer_ = players_[selectedPlayerNum_];

    // Search next player
    while (!currentPlayer_->hasTowns()) {
        delete currentPlayer_;
        players_.erase(players_.begin() + selectedPlayerNum_);
        if (players_.empty()) return;

        selectedPlayerNum_ %= players_.size();
        currentPlayer_ = players_[selectedPlayerNum_];
    }

    if (selectedPlayerNum_ == 0) {
        moveBandits();
        for (Player *player : players_)
            updateIncomes(player);
    }

    // Start turn of new current player
    movedTroops_.clear();
    currentPlayer_->onTurnStart();
    updateIncomes(currentPlayer_);
    refreshElements();
}


void GameMap::selectCell(const Point& pos) {
    //! update le code de cette fonction
    double islandInnerRadius = Ground::getInnerRadius();
    double islandRadius = Ground::getRadius();
    
    int relX = pos.getX() / ratio_ - islandInnerRadius;
    int relY = pos.getY() / ratio_ - islandRadius;

    auto [q, r] = HexagonUtils::pixelToAxial(relX, relY, islandRadius);
    auto [x, y] = HexagonUtils::axialToOffset(q, r);
    
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) {
        selectedCell_.reset();
        return;
    }
    
    auto pg = dynamic_cast<PlayableGround*>(get(x, y));
    selectedCell_ = pg ? pg : nullptr;
}

void GameMap::updateSelectedCell() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    selectCell(Point{x, y} - pos_);
}

//! Pour l'ajout de troupe depuis le shop
//void GameMap::addTroop(Troop* fromTroop, PlayableGround* to)

void GameMap::moveTroop(PlayableGround* from, PlayableGround* to) {
    if (!from || !to || from == to || !to->isSelectable()) return;

    // Get interesting elements
    auto fromTroop = dynamic_cast<Troop*>(from->getElement());
    if (!fromTroop) return;
    
    auto toTroop = dynamic_cast<Troop*>(to->getElement());
    auto fromOwner = from->getOwner();
    auto toOwner = to->getOwner();


    // Different owner
    if (fromOwner != toOwner || dynamic_cast<Bandit*>(toTroop)) {
        delete toTroop;
        from->setElement(nullptr);
        to->setElement(fromTroop);

        movedTroops_.insert(fromTroop);

        // Give grounds to the new owner
        to->link(fromOwner);
        updateIncomes(fromOwner);
        if (toOwner) updateIncomes(toOwner);
    } 
    
    // Same Owner
    else {
        if (!to->getElement()) {
            from->setElement(nullptr);
            to->setElement(fromTroop);
        }

        else if (toTroop) {
            // Check merge
            Troop *troop = nullptr;
            if (dynamic_cast<Villager*>(fromTroop) && dynamic_cast<Villager*>(toTroop))
                troop = new Pikeman(to->getPos());
            else if (dynamic_cast<Pikeman*>(fromTroop) && dynamic_cast<Pikeman*>(toTroop))
                troop = new Knight(to->getPos());
            else if (dynamic_cast<Knight*>(fromTroop) && dynamic_cast<Knight*>(toTroop)) 
                troop = new Hero(to->getPos());

            if (troop) {
                // Merge troops
                if (movedTroops_.find(fromTroop) != movedTroops_.find(toTroop))
                    movedTroops_.insert(troop);

                delete fromTroop;
                delete toTroop;

                from->setElement(nullptr);
                to->setElement(troop);

                updateIncomes(fromOwner);
            }
        }
    }
}

void GameMap::moveBandit(PlayableGround* from, PlayableGround* to) {
    if (!from || !to || from == to) return;

    // Get interesting elements
    auto fromBandit = dynamic_cast<Bandit*>(from->getElement());
    if (!fromBandit) return;
    
    if (to->getElement()) return;
    
    // Move bandit
    from->setElement(nullptr);
    to->setElement(fromBandit);
}

void GameMap::moveBandits() {
    std::unordered_set<Bandit*> movedBandits;

    for (Cell* cell : *this) {
        auto* pg = dynamic_cast<PlayableGround*>(cell);
        if (!pg) continue;

        auto* bandit = dynamic_cast<Bandit*>(pg->getElement());
        if (!bandit || movedBandits.find(bandit) != movedBandits.end()) continue;

        // Get free neighbors
        std::vector<PlayableGround*> candidates;
        for (Cell* c : pg->getNeighbors())
            if (auto* g = dynamic_cast<PlayableGround*>(c))
                if (!g->getElement() && !g->hasFences())
                    candidates.push_back(g);


        // Random move bandit
        if (!candidates.empty()) {
            std::uniform_int_distribution<> dist(0, static_cast<int>(candidates.size()) - 1);
            PlayableGround* dest = candidates[dist(gen_)];
            moveBandit(pg, dest);
            movedBandits.insert(bandit);
        }
    }

    refreshElements();
}


bool GameMap::isSelectableTroop(PlayableGround* pg) {
    if (!pg) return false;

    // Check owner
    auto owner = pg->getOwner();
    if (!(owner && owner->hasSelected())) return false;

    // Check troop
    auto troop = dynamic_cast<Troop*>(pg->getElement());
    if (!troop || dynamic_cast<Bandit*>(troop)) return false;
    if (movedTroops_.find(troop) != movedTroops_.end()) return false;

    return true;
}

void GameMap::updateCursor() {
    if (selectedCell_.has_value() && isSelectableTroop(*selectedCell_))
        SDL_SetCursor(handCursor_);
    else
        SDL_SetCursor(arrowCursor_);
}

void GameMap::handleEvent(SDL_Event &event) {
    //! Voir si faire fonction pour chaque event
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: {
            selectedTroopCell_ = nullptr;
            selectedTroop_ = nullptr;
            if (!selectedCell_.has_value() || !isSelectableTroop(*selectedCell_))
                break;
            
            // Select element
            selectedTroopCell_ = *selectedCell_;
            selectedTroop_ = dynamic_cast<Troop*>(selectedTroopCell_->getElement());

            selectedTroopCell_->updateSelectable(selectedTroop_->getStrength());
            selectedTroopCell_->setElement(nullptr);
            
            refreshElements();
            refreshSelectables();

            // No break
        }

        case SDL_MOUSEMOTION: {
            Point mousePos{event.motion.x, event.motion.y};
            selectCell(mousePos - pos_);
            townToShowTreasury_ = nullptr;

            if (selectedTroop_) {
                selectedTroop_->setPos(mousePos);
            }
            else {
                updateCursor();
                if (selectedCell_.has_value() && (*selectedCell_)) {
                    auto town = dynamic_cast<Town*>((*selectedCell_)->getElement());
                    if (town)
                        townToShowTreasury_ = town;
                }
            }
            refreshElements();

            break;
        }

        case SDL_MOUSEBUTTONUP: {
            if (selectedTroopCell_ && selectedTroop_) {
                selectedTroopCell_->setElement(selectedTroop_);

                // Move troop
                if (selectedCell_.has_value() && (*selectedCell_))
                    moveTroop(selectedTroopCell_, *selectedCell_);

                // Remove possibilities
                selectedTroopCell_->updateSelectable(0);
                selectedTroopCell_ = nullptr;
                selectedTroop_ = nullptr;
                refresh();
            }
            updateCursor();
            break;
        }
    }
}

void GameMap::display(const BlitTarget* target)
{
    if (!islandsCalc_ || !platesCalc_ || !selectablesCalc_ || !fencesCalc_ || !elementsCalc_)
        refresh();

    //! encapsuler renderer dans une classe
    Rect dest = {pos_, size_};
    target->blit(calc_, dest);

    if (selectedTroop_)
        selectedTroop_->display(target);
}

void GameMap::updateIncomes(Player *player) {
    for (auto* pg : player->getTownCells())
        if (auto* town = dynamic_cast<Town*>(pg->getElement()))
            town->setIncome(0);

    for (Cell* cell : *this)
        if (auto* pg = dynamic_cast<PlayableGround*>(cell))
            if (pg->getOwner() == player)
                pg->updateIncome();
}
