#ifndef UNIT_TOWN_HPP
#define UNIT_TOWN_HPP

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"    // Interface for anything that can be blitted onto

//------------------------------
// Display Helpers
//------------------------------
#include "Displayers/TreasuryDisplayer.hpp"      // Renders the town’s treasury amount on screen

//------------------------------
// Core Game Element
//------------------------------
#include "GameElements/GameElement.hpp"          // Base class for all game entities

/**
 * @brief Represents a settlement that can hold and display currency.
 *
 * Inherits from GameElement, providing position, lifecycle,
 * and interaction behaviors common to all elements.
 */
class Town : public GameElement {
public:
    /**
     * @brief Load shared resources for all Town instances.
     * Must be called before creating any Town.
     */
    static void init();

    /**
     * @brief Release shared resources.
     * Call during shutdown to free textures, etc.
     */
    static void quit();

    /**
     * @brief Safely cast a generic GameElement to a Town.
     * @param obj Weak pointer to the element.
     * @return Shared pointer to Town or nullptr if not a Town.
     */
    static std::shared_ptr<Town> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a generic GameElement is a Town.
     * @param obj Weak pointer to the element.
     * @return true if obj wraps a Town instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    std::shared_ptr<GameElement> deepCopy() const override;

    /**
     * @brief Construct a Town at a given position with an initial treasury.
     * @param pos Center point in pixel coordinates.
     * @param treasury Initial amount of coins stored.
     */
    explicit Town(const Point& pos, const int& treasury = 0);

    /** @name Display Overrides
     *  @{
     */
    /**
     * @brief Update the Town’s position.
     * @param pos New center point in pixel coordinates.
     */
    void setPos(const Point& pos) override;

    /**
     * @brief Render the town sprite onto the given BlitTarget.
     * Overrides the pure virtual in Displayer.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;
    /** @} */

    /**
     * @brief Render the treasury amount UI for this town.
     * @param target Weak pointer to the render target.
     */
    void displayTreasury(const std::weak_ptr<BlitTarget>& target);

    /* --- Generic Accessors (GameElement interface) --- */
    const int getStrength() const override;  ///< Returns the unit strength (always 1 for Town).
    const int getCost()     const override;  ///< Returns the deployment cost (0 for Town).
    const int getUpkeep()   const override;  ///< Returns the upkeep per turn (0 for Town).

    /* --- Treasury Management --- */
    const int getTreasury() const;            ///< Get current coin count.
    void setTreasury(int treasury);           ///< Set coin count explicitly.
    void updateTreasury();                    ///< Apply income to treasury at end of turn.

    /* --- Income Handling --- */
    const int getIncome() const;              ///< Get coins gained per turn.
    void setIncome(int income);               ///< Set the income rate.
    void addIncome(int coins);                ///< Increase income by a flat amount.

    /**
     * @brief Toggle visual selection state.
     * @param selected true to render selection highlight.
     */
    void setSelected(bool selected);

protected:
    static constexpr int STRENGTH = 1;  ///< Strength value for combat.
    static constexpr int COST     = 0;  ///< Deployment cost in coins.
    static constexpr int UPKEEP   = 0;  ///< Upkeep cost per turn.

private:
    /// Sprite texture for the town graphic.
    static std::shared_ptr<Texture> sprite_;
    /// Sprite texture used when the town is selected.
    static std::shared_ptr<Texture> selectSprite_;

    TreasuryDisplayer treasuryDisplayer_;  ///< Helper for drawing the treasury UI
    bool selected_     = false;           ///< Whether the town is highlighted
    int  treasury_     = 0;               ///< Current coins stored
    int  income_       = 0;               ///< Coins gained at end of each turn
};

#endif // UNIT_TOWN_HPP
