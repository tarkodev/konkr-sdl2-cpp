#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"                                      // Core SDL types and event handling

//------------------------------
// Widget & Grid
//------------------------------
#include "Widgets/HexagonGrid.hpp"                    // Hexagon-based grid container

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"          // Wrapper for SDL_Texture
#include "SDLWrappers/Coords/Rect.hpp"                // Integer rectangle type
#include "SDLWrappers/Coords/Point.hpp"               // Integer point type
#include "SDLWrappers/Coords/Size.hpp"                // Width/height pair
#include "SDLWrappers/Font.hpp"                       // Text rendering support
#include "SDLWrappers/Renderers/BlitTarget.hpp"       // Interface for render targets

//------------------------------
// Game Elements
//------------------------------
#include "Cells/Cell.hpp"                             // Base grid cell
#include "GameElements/Player.hpp"                    // Player logic and state
#include "GameElements/Troops/Troop.hpp"              // Troop units
#include "GameElements/Castle.hpp"                    // Castle structures
#include "GameElements/Camp.hpp"                      // Camp structures

//------------------------------
// UI Displayers
//------------------------------
#include "Displayers/Displayer.hpp"                   // Abstract renderable interface
#include "Displayers/TreasuryDisplayer.hpp"           // Shows player treasury info

//------------------------------
// STL & Utilities
//------------------------------
#include <memory>                                     // std::shared_ptr, std::weak_ptr
#include <utility>                                    // std::pair
#include <optional>                                   // std::optional
#include <vector>                                     // std::vector
#include <algorithm>                                  // std algorithms
#include <random>                                     // Random number generator
#include <unordered_map>                              // Hash map container
#include <unordered_set>                              // Hash set container

/**
 * @brief Represents the main game map containing cells, elements, and rendering logic.
 *
 * Inherits from HexagonGrid<std::shared_ptr<Cell>> to manage a grid of Cell pointers,
 * and from Displayer to provide a display() implementation.
 */
class GameMap : public HexagonGrid<std::shared_ptr<Cell>>, public Displayer {
public:
    /** @brief Load shared resources (textures, fonts) needed by all GameMap instances. */
    static void init();

    /**
     * @brief Factory: create a Cell subclass based on a character code.
     * @param letter Code identifying cell type.
     * @param pos   Cell origin in map coordinates.
     * @return Shared pointer to new Cell.
     */
    static std::shared_ptr<Cell> createCell(char letter, Point pos);

    /**
     * @brief Factory: create a GameElement (troop, castle, camp, etc.) from a code.
     * @param letter Code identifying element type.
     * @param pos    Element position in pixels.
     * @return Shared pointer to new GameElement.
     */
    static std::shared_ptr<GameElement> createGameElement(char letter, Point pos);

    /**
     * @brief Construct a GameMap by loading layout from a file.
     * @param pos     Top-left corner in world coords.
     * @param size    Pixel dimensions.
     * @param mapFile Path to map description file.
     */
    GameMap(const Point& pos, const Size size, const std::string mapFile);

    /** @brief Adjust the displayed size proportionally when the window is resized. */
    void setProportionalSize(const Size size);

    /** @brief Selects the cell under a given point (e.g., mouse click). */
    void selectCell(const Point& pos);

    /** @brief Move the map to a new top-left position. */
    void setPos(const Point& pos) override;

    /** @brief Redraw all map components (terrain, elements, overlays). */
    void refresh() const;

    /** @brief Return the current player's maximum treasury (for UI display). */
    const int getMaxTreasuryOfCurrentPlayer();

    /** @brief Attempt to purchase a troop for the current player. */
    void buyTroop(const std::shared_ptr<GameElement>& elt);

    /** @brief Check if there is an actively selected troop. */
    const bool hasTroopSelected();

    /** @brief Handle SDL input events (mouse, keyboard). */
    void handleEvent(SDL_Event &event);

    /** @brief Render the entire map onto the given target. */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /** @brief Check whether the win/lose conditions have been met. */
    const bool gameFinished() const;

    /** @brief Advance to the next player's turn. */
    void nextPlayer();

    /** @brief Return the pixel width of the map area. */
    const int getWidth() const override;

    /** @brief Return the pixel height of the map area. */
    const int getHeight() const override;

private:
    static std::mt19937 gen_;                                     ///< Random number generator

    double ratio_ = 0;                                            ///< Scale factor for drawing

    std::weak_ptr<PlayableGround> selectedCell_;                  ///< Currently selected ground cell
    std::weak_ptr<Town> townToShowTreasury_;                      ///< Town whose treasury is visible
    std::weak_ptr<Camp> campToShowTreasury_;                      ///< Camp whose treasury is visible
    bool hasSelection_ = false;                                   ///< Flag for any selection

    std::vector<std::weak_ptr<Player>> players_;                  ///< All players in the game
    std::weak_ptr<Player> currentPlayer_;                         ///< Player whose turn it is

    std::vector<std::weak_ptr<Troop>> movedTroops_;               ///< Troops moved this turn
    std::shared_ptr<GameElement> boughtElt_;                      ///< Last purchased element
    std::shared_ptr<Troop> selectedTroop_;                        ///< Currently selected troop
    std::weak_ptr<PlayableGround> selectedTroopCell_;             ///< Cell of selected troop
    std::shared_ptr<PlayableGround> selectedNewTroopCell_;        ///< Destination for a troop move
    std::unordered_set<std::shared_ptr<PlayableGround>> potentialTownCells_; ///< Valid town placement cells
    int selectedPlayerNum_ = 0;                                   ///< Index of current player

    Size calcSize_;                                               ///< Size of calculation overlay
    std::shared_ptr<Texture> calc_ = nullptr;                     ///< Texture for overlays
    bool gameFinished_ = false;                                   ///< Game-over flag

    /**
     * @brief Internal constructor that unifies both creation paths.
     * @param pos      Map position.
     * @param size     Display size.
     * @param gridSize Grid dimensions.
     * @param mapFile  Optional file path (empty for manual grid).
     */
    GameMap(const Point& pos, const Size size, const std::pair<int, int>& gridSize, const std::string mapFile);

    /** @brief Load map data (cell types, elements) from a file. */
    void loadMap(const std::string& mapFile);

    /** @brief Determine grid dimensions by reading the map file. */
    std::pair<int,int> getSizeOfMapFile(const std::string& mapFile);

    /** @brief Update adjacency links between neighboring cells. */
    void updateNeighbors();

    /** @brief Generate textures for calculation overlays. */
    void createCalcs();

    /** @brief Refresh the visual highlight on the selected cell. */
    void updateSelectedCell();

    /** @brief Check if a troop can be selected on a given ground. */
    const bool isSelectableTroop(const std::weak_ptr<PlayableGround>& pg);

    /** @brief Update mouse cursor icon based on context. */
    void updateCursor();

    /** @brief Find the next player who has available moves/resources. */
    void searchNextPlayer();

    /** @brief Recompute links between moved elements and their targets. */
    void updateLinks();

    /** @brief Initialize all game-state variables for a new game. */
    void initGame();

    /** @brief Charge bandit fee or remove bandits from a player if resources allow. */
    void defrayBandits(const std::weak_ptr<Player>& player);

    /** @brief Enforce debt/payment deficits before turn start. */
    void checkDeficits(std::weak_ptr<Player>& player);

    /** @brief Set up state for the given player's turn. */
    void startTurn(std::weak_ptr<Player>& player);

    /** @brief Check if a troop has already moved this turn. */
    const bool isMovedTroop(const std::weak_ptr<Troop>& troop);

    /** @brief Remove elements that have been destroyed or lost. */
    void updateLostElements();

    /** @brief Replenish troops available to a player at start of turn. */
    void updateFreeTroops(const std::weak_ptr<Player>& player);

    /** @brief Count treasures in cells owned by the current player. */
    std::unordered_map<std::shared_ptr<PlayableGround>, int> getTreasuresOfCurrentPlayers();

    /** @brief Place a new castle on a ground cell, if legal. */
    const bool placeCastle(const std::weak_ptr<Castle>& castle, const std::weak_ptr<PlayableGround>& to);

    /** @brief Move a troop from one cell to another. */
    void moveTroop(const std::weak_ptr<PlayableGround>& from, const std::weak_ptr<PlayableGround>& to);

    /** @brief Move a single bandit from one cell to another. */
    void moveBandit(const std::weak_ptr<PlayableGround>& from, const std::weak_ptr<PlayableGround>& to);

    /** @brief Relocate all bandits according to AI rules. */
    void moveBandits();

    /** @brief Update resource income for a player at end of turn. */
    void updateIncomes(std::weak_ptr<Player>& player);
    void updateIncomes(std::shared_ptr<Player>& player);

    /** @brief Mark movable elements based on current state. */
    void updateMovables();

    /** @brief Check if victory conditions have been met. */
    void checkWin();

    // Mouse event callbacks
    void onMouseButtonDown(SDL_Event& event);
    void onMouseMotion(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    // Rendering helpers
    void refreshIslands() const;
    void refreshPlates() const;
    void refreshSelectables() const;
    void refreshFences() const;
    void refreshElements() const;
};

#endif // GAMEMAP_HPP
