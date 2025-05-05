#ifndef PLAYER_HPP
#define PLAYER_HPP

//------------------------------
// Standard Library
//------------------------------
#include <string>             // std::string for player identifiers or names
#include <memory>             // std::shared_ptr, std::weak_ptr
#include <unordered_set>      // std::unordered_set for collections of grounds
#include <vector>             // std::vector for town cell list

//------------------------------
// SDL2 Rendering
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"   // Encapsulates SDL_Texture with RAII semantics

//------------------------------
// Display Utilities
//------------------------------
#include "Displayers/HexagonDisplayer.hpp"     // Renders hexagonal plate overlays

//------------------------------
// Game Utilities
//------------------------------
#include "Utils/ColorUtils.hpp"                // Defines GroundColor and color utilities
using namespace ColorUtils;                    // Bring GroundColor into current namespace

//------------------------------
// Forward Declarations
//------------------------------
class PlayableGround;                         // Represents a cell the player can own
#include "GameElements/Town.hpp"             // Town class for tracking player's towns

/**
 * @brief Represents a player in the game.
 *
 * Manages the player's color, owned towns, turn state, and rendering plates.
 */
class Player {
public:
    /**
     * @brief Initialize shared resources for all Player instances.
     * Loads the static HexagonDisplayer plate graphics.
     * @param renderer Shared SDL_Renderer for texture creation.
     */
    static void init(const std::shared_ptr<SDL_Renderer>& renderer);

    /**
     * @brief Release shared resources when the program ends.
     * Frees the static HexagonDisplayer plates.
     */
    static void quit();

    /**
     * @brief Construct a Player with a given color scheme.
     * @param color Pair of owned/available colors from ColorUtils::GroundColor.
     */
    explicit Player(const GroundColor& color);

    /**
     * @brief Destructor: cleans up any remaining player state.
     */
    ~Player();

    /**
     * @brief Get the colored hex plate for this player.
     * @return Reference to the owned-plate displayer.
     */
    HexagonDisplayer& getPlate();

    /**
     * @brief Get the “lost” (available) hex plate for this player.
     * @return Reference to the available-plate displayer.
     */
    HexagonDisplayer& getLostPlate();

    /**
     * @brief Remove entries for towns that have been destroyed.
     * Called at start of turn to prune stale town records.
     */
    void updateTowns();

    /**
     * @brief Check if the player has been selected for this turn.
     * @return True if onTurnStart was called without a matching onTurnEnd.
     */
    const bool hasSelected() const { return selected_; };

    /**
     * @brief Determine if the player currently owns at least one town.
     * @return True if any townCells_ entry still contains a Town.
     */
    const bool hasTowns();

    /**
     * @brief Access the list of owned town cells.
     * @return Vector of weak_ptr to PlayableGrounds containing towns.
     */
    std::vector<std::weak_ptr<PlayableGround>> getTownCells();

    /**
     * @brief Add a new town cell to this player's ownership.
     * @param town Weak pointer to the PlayableGround hosting the town.
     */
    void addTownCell(std::weak_ptr<PlayableGround> town);

    /**
     * @brief Mark the start of this player's turn.
     * Sets selected_, highlights towns, and resets incomes.
     */
    void onTurnStart();

    /**
     * @brief Mark the end of this player's turn.
     * Unhighlights towns and clears selected_.
     */
    void onTurnEnd();

private:
    //------------------------------
    // Shared Renderer & Plate
    //------------------------------
    static std::weak_ptr<SDL_Renderer> renderer_;  // Renderer for loading textures
    static HexagonDisplayer plateDisplayer_;       // Prototype plate displayer

    //------------------------------
    // Per-Player State
    //------------------------------
    GroundColor color_;                            // This player's color scheme

    HexagonDisplayer plate_;                       // Colored owned-plate instance
    HexagonDisplayer lostPlate_;                   // Colored available-plate instance

    std::vector<std::weak_ptr<PlayableGround>> allTownCells_; // Cells containing this player's all towns
    std::vector<std::weak_ptr<PlayableGround>> townCells_; // Cells containing this player's towns

    bool selected_ = false;                        // True while it's this player's turn
};

#endif // PLAYER_HPP
