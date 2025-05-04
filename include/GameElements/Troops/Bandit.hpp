#ifndef UNIT_BANDIT_HPP
#define UNIT_BANDIT_HPP

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"  // Interface for objects that can be blitted onto a render target

//------------------------------
// Game Logic: Troop Hierarchy
//------------------------------
#include "GameElements/Troops/Troop.hpp"        // Base class for all troop elements

/**
 * @brief Bandit troop: low-strength nuisance unit.
 *
 * Inherits from Troop. Bandits roam the map, can capture empty camps,
 * and collect coins for those camps at turn end.
 */
class Bandit : public Troop {
public:
    /**
     * @brief Safely cast a generic GameElement to a Bandit.
     * @param obj Weak pointer to a GameElement.
     * @return Shared pointer to Bandit, or nullptr if the cast fails.
     */
    static std::shared_ptr<Bandit> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a generic GameElement is actually a Bandit.
     * @param obj Weak pointer to a GameElement.
     * @return true if obj wraps a Bandit instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);
    
    /**
     * @brief Load shared Bandit resources (sprite).
     * Must be called once before creating any Bandit instances.
     */
    static void init();

    /**
     * @brief Release shared Bandit resources.
     * Call during shutdown to free memory.
     */
    static void quit();

    /**
     * @brief Construct a Bandit at the given position.
     * @param pos Center point in pixel coordinates.
     */
    explicit Bandit(const Point& pos);

    /**
     * @brief Render the bandit sprite onto the given BlitTarget.
     * Overrides Troop::display.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /* --- Generic Accessors --- */

    /// Combat strength of the bandit (always 0).
    const int getStrength() const override { return STRENGTH; };
    /// Recruitment cost of the bandit (always 0).
    const int getCost()     const override { return COST; };
    /// Upkeep cost per turn for the bandit (always 1).
    const int getUpkeep()   const override { return UPKEEP; };

protected:
    static constexpr int STRENGTH = 0;  ///< Bandit's combat strength.
    static constexpr int COST     = 0;  ///< Resource cost to recruit.
    static constexpr int UPKEEP   = 1;  ///< Resource upkeep per turn.

private:
    /// Shared sprite texture for all Bandit instances.
    static std::shared_ptr<Texture> sprite_;
};

#endif // UNIT_BANDIT_HPP
