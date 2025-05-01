#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

//! trier du plus bas au plus haut niveau (SDL est l'intermédiaire)
#include "SDL.h"
#include "HexagonGrid.hpp"
#include "Texture.hpp"
#include "Cell.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Player.hpp"
#include "logic/Troop.hpp"
#include "Displayer.hpp"
#include "BlitTarget.hpp"
#include "Font.hpp"
#include "TreasuryDisplayer.hpp"

#include <memory>
#include <utility>
#include <optional>
#include <vector>
#include <algorithm>
#include <random>

/**
 * @brief Classe représentant une carte de jeu.
 *
 * Hérite de HexagonGrid<int> pour gérer la logique de la grille d'hexagons,
 * et ajoute des fonctionnalités de rendu graphique (création de sprites, etc.).
 */
class GameMap : public HexagonGrid<Cell*>, public Displayer {
public:
    static void init();
    
    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize);
    GameMap(const Point& pos, const Size size, const std::string mapFile);

    const Size getSize() const override;
    void setProportionalSize(const Size size);
    void selectCell(const Point& pos);

    void setPos(const Point& pos) override;

    void refreshIslands();
    void refreshPlates();
    void refreshSelectables();
    void refreshFences();
    void refreshElements();
    void refresh();

    bool hasTroopSelected() {return selectedTroopCell_ != nullptr;};

    void handleEvent(SDL_Event &event);
    void display(const BlitTarget* target) override;

    void nextPlayer();

private:
    static std::mt19937 gen_;
    static Texture* selectSprite_; //!temp
    static SDL_Cursor* handCursor_;
    static SDL_Cursor* arrowCursor_;

    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize, const std::string mapFile);
    void loadMap(const std::string& mapFile);
    std::pair<int,int> getSizeOfMapFile(const std::string& mapFile);

    void updateNeighbors();
    void createCalcs();
    void updateSelectedCell();
    bool isSelectableTroop(PlayableGround* pg);
    void updateCursor();
    void refreshMain();
    void initGame();
    void defrayBandits(Player *player);
    void checkDeficits(Player *player);

    void updateLostElements();
    void updateFreeTroops(Player *player);
    void moveTroop(PlayableGround* from, PlayableGround* to);
    void moveBandit(PlayableGround* from, PlayableGround* to);
    void moveBandits();
    void updateIncomes(Player *player);

    double ratio_ = 0;

    std::optional<PlayableGround*> selectedCell_; //! devrait être un std::optional<PlayableGround> ?
    Town* townToShowTreasury_;
    bool hasSelection_ = false;

    std::vector<Player*> players_;
    Player* currentPlayer_;

    std::unordered_set<Troop *> movedTroops_;
    Troop* selectedTroop_ = nullptr;
    PlayableGround* selectedTroopCell_ = nullptr;
    int selectedPlayerNum_ = 0;

    Size size_;
    Size calcSize_;
    Texture* islandsCalc_ = nullptr; //! tout passer en unique_ptr
    Texture* platesCalc_ = nullptr;
    Texture* selectablesCalc_ = nullptr;
    Texture* fencesCalc_ = nullptr;
    Texture* elementsCalc_ = nullptr;
    Texture* calc_ = nullptr;
};

#endif
