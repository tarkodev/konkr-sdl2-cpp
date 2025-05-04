#ifndef OVERLAY_HPP
#define OVERLAY_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"                  // Provides SDL_Event, SDL_Point, etc.

//------------------------------
// STL & Memory Management
//------------------------------
#include <vector>                // std::vector for collections
#include <memory>                // std::shared_ptr, std::unique_ptr

//------------------------------
// Coordinate Abstractions
//------------------------------
#include "SDLWrappers/Coords/Point.hpp"    // Encapsulates x,y position

//------------------------------
// Rendering & Widgets
//------------------------------
#include "Displayers/Displayer.hpp"        // Base class for renderable objects
#include "Widgets/Button.hpp"              // UI button widget

//------------------------------
// Game Elements
//------------------------------
#include "GameElements/Troops/Troop.hpp"   // Forward declares Troop type

/**
 * @brief Represents a selectable troop option in the overlay.
 *
 * Contains textures for purchase state, button for buying,
 * and helpers for position/size retrieval.
 */
struct TroopOption {
    std::shared_ptr<Texture>   texNoBuy;   ///< Texture when troop is not buyable
    Point                      texPos;     ///< Position to draw the texture
    std::unique_ptr<Button>    btnBuy;     ///< Button to initiate purchase
    bool                       buyable = false; ///< Whether the troop can currently be bought

    /// Get the top-left position of the buy button.
    const Point getPos() const { return btnBuy->getPos(); }

    /// Get the dimensions of the buy button.
    Size getSize() const { return btnBuy->getSize(); }
};

/**
 * @brief An overlay UI drawn above the game map.
 *
 * Displays buttons for navigating back, ending turn,
 * and buying troops. Handles input events and purchase logic.
 */
class Overlay : public Displayer {
public:
    /**
     * @brief Construct an Overlay at a given position.
     * @param pos Top-left coordinate for the overlay background.
     */
    explicit Overlay(const Point& pos);

    /// Default destructor.
    ~Overlay() = default;

    /**
     * @brief Process incoming SDL events (mouse clicks, movement).
     * @param e Event to handle.
     */
    void handleEvent(const SDL_Event& e);

    /**
     * @brief Check if the mouse is hovering over any interactive area.
     * @param mousePos Current mouse coordinates.
     * @return true if hovering over a button.
     */
    const bool isHover(const Point& mousePos) const;

    /**
     * @brief Move the overlay to a new position.
     * Overrides Displayer::setPos.
     * @param pos New top-left coordinate.
     */
    void setPos(const Point& pos) override;

    /**
     * @brief Render the overlay background and all buttons/options.
     * Overrides Displayer::display.
     * @param target Render target (e.g., window or texture).
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /**
     * @brief Query if the back button was pressed.
     * @return true if user requested to go back.
     */
    const bool backRequested();

    /**
     * @brief Query if the turn-end button was pressed.
     * @return true if user requested to end their turn.
     */
    const bool turnRequested();

    /**
     * @brief Query if a troop purchase was requested.
     * @return true if a buy action occurred this frame.
     */
    const bool buyTroopRequested();

    /**
     * @brief Retrieve the troop GameElement corresponding to the last purchase.
     * @return Shared pointer to the newly bought troop.
     */
    std::shared_ptr<GameElement> getTroopBought();

    /**
     * @brief Update overlay state based on current treasury amount.
     * Enables/disables purchase buttons accordingly.
     * @param treasury Current player gold count.
     */
    void update(int treasury);

private:
    std::shared_ptr<Texture>       bg_;           ///< Background texture for the overlay
    std::unique_ptr<Button>        backBtn_;      ///< Button to go back to main menu
    std::vector<TroopOption>       options_;      ///< List of troop purchase options
    std::unique_ptr<Button>        turnBtn_;      ///< Button to end the current turn
    Point                          bgPos_;        ///< Top-left position of the background

    bool backRequested_    = false;    ///< Flag set when backBtn_ is clicked
    bool turnRequested_    = false;    ///< Flag set when turnBtn_ is clicked
    char buyTroopRequested_ = '\0';     ///< ID of troop requested for purchase (or '\0' if none)
};

#endif // OVERLAY_HPP
