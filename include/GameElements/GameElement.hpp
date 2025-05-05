#ifndef LOGIC_GAMEELEMENT_HPP
#define LOGIC_GAMEELEMENT_HPP

//------------------------------
// Standard Library
//------------------------------
#include <string>  // std::string used for potential naming or identification

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for blitting textures onto a render target

//------------------------------
// Display Abstractions
//------------------------------
#include "Displayers/Displayer.hpp" // Base class providing position, size, and renderer reference

// Forward declaration to avoid circular dependency.
// GameElement may need to interact with Player (e.g., ownership, buffs, etc.).
class Player;

/**
 * @brief Logical element placed on a Cell (unit, building, neutral hazard, etc.).
 *
 * Inherits from Displayer, so it has a position and rendering capabilities.
 * Provides pure virtual methods for strength, cost, upkeep, and a display function.
 * Tracks a 'lost' state to indicate destruction or removal from the game.
 */
class GameElement : public Displayer {
public:
    /// Virtual destructor to allow proper cleanup in derived classes.
    virtual ~GameElement() = default;

    /* --- Generic Accessors --- */

    /**
     * @brief Get the combat or interaction strength of this element.
     * @return Integer strength value used in combat calculations.
     */
    virtual const int getStrength() const = 0;

    /**
     * @brief Get the purchase or build cost of this element.
     * @return Integer cost value deducted from player treasury.
     */
    virtual const int getCost() const = 0;

    /**
     * @brief Get the upkeep cost of this element per turn.
     * @return Integer upkeep value deducted each turn.
     */
    virtual const int getUpkeep() const = 0;

    /* --- Rendering --- */

    /**
     * @brief Render the element onto a BlitTarget.
     * Must be implemented by derived classes to draw their specific sprite or representation.
     * @param target Weak pointer to the render target (window or texture).
     */
    virtual void display(const std::weak_ptr<BlitTarget>& target) const override = 0;

    /**
     * @brief Check if the element has been marked as lost (destroyed).
     * @return true if lost, false otherwise.
     */
    virtual const bool isLost() const { return lost_; };

    /**
     * @brief Mark the element as lost. Subsequent rendering may show a 'lost' sprite.
     */
    virtual void lost() { lost_ = true; };

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    virtual std::shared_ptr<GameElement> deepCopy() const = 0;

protected:
    /**
     * @brief Protected constructor to enforce abstract usage.
     * @param pos Center position in pixel coordinates.
     * @param size Dimensions of the element's sprite or bounding box.
     */
    GameElement(const Point& pos, const Size& size);

    // Default values for derived classes that don't override these.
    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;

    /// Internal flag indicating whether this element has been destroyed.
    bool lost_ = false;
};

#endif // LOGIC_GAMEELEMENT_HPP
