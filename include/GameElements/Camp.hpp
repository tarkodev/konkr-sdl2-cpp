#ifndef UNIT_CAMP_HPP
#define UNIT_CAMP_HPP

//------------------------------
// Rendering Target Abstraction
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for objects that can be blitted onto

//------------------------------
// Game Logic Base Class
//------------------------------
#include "GameElements/GameElement.hpp"         // Base class for logical game elements with display capabilities

//------------------------------
// Treasury Display Utility
//------------------------------
#include "Displayers/TreasuryDisplayer.hpp"     // Renders the camp’s treasury UI overlay

/**
 * @brief Represents a resource camp on the map.
 *
 * A Camp holds coins (treasury) and displays its sprite and treasury.
 * Inherits from GameElement, so it can be treated like any other logical element.
 */
class Camp : public GameElement {
public:
    /**
     * @brief Safely cast a generic GameElement to a Camp.
     * @param obj Weak pointer to the game element.
     * @return Shared pointer to Camp or nullptr if the cast fails.
     */
    static std::shared_ptr<Camp> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a generic GameElement is actually a Camp.
     * @param obj Weak pointer to the game element.
     * @return true if obj wraps a Camp instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    std::shared_ptr<GameElement> deepCopy() const override;

    /**
     * @brief Load shared resources (sprite) for all Camp instances.
     * Must be called once during initialization.
     */
    static void init();

    /**
     * @brief Release shared resources (sprite) before shutdown.
     */
    static void quit();

    /**
     * @brief Construct a Camp at the given position with an optional initial treasury.
     * @param pos Center point in pixel coordinates.
     * @param treasury Initial coin count (default 0).
     */
    explicit Camp(const Point& pos, const int& treasury = 0);

    /**
     * @name Displayer Overrides
     * @{
     */

    /**
     * @brief Update the camp’s position (and its treasury display).
     * @param pos New center point in pixel coordinates.
     */
    void setPos(const Point& pos) override;

    /**
     * @brief Render the camp sprite on the target.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /** @} */

    /**
     * @name Generic GameElement Accessors
     * @{
     */

    /// Combat strength of the camp (always 1).
    const int getStrength() const override { return STRENGTH; };

    /// Resource cost to build a camp (always 0).
    const int getCost()     const override { return COST; };

    /// Upkeep cost per turn for the camp (always 1).
    const int getUpkeep()   const override { return UPKEEP; };

    /** @} */

    /**
     * @brief Add coins to the camp’s treasury.
     * @param coins Number of coins to add.
     */
    void addCoins(int coins);

    /**
     * @brief Get the current treasury amount.
     * @return Number of coins stored in the camp.
     */
    const int getTreasury() const;

    /**
     * @brief Render the treasury overlay (coin count and income).
     * @param target Weak pointer to the render target.
     */
    void displayTreasury(const std::weak_ptr<BlitTarget>& target);

protected:
    /// Fixed strength value for all camps.
    static constexpr int STRENGTH = 1;
    /// Fixed build cost for all camps.
    static constexpr int COST     = 0;
    /// Fixed upkeep cost for all camps.
    static constexpr int UPKEEP   = 1;

private:
    /// Shared static sprite for all Camp instances.
    static std::shared_ptr<Texture> sprite_;

    /// Handles the visual display of the camp’s treasury (background + text).
    TreasuryDisplayer treasuryDisplayer_;

    /// Current coin count stored in this camp.
    int treasury_ = 0;
};

#endif // UNIT_CAMP_HPP
