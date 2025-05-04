#ifndef UNIT_HERO_HPP
#define UNIT_HERO_HPP

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"  // Interface for anything that can be blitted onto

//------------------------------
// Game Logic
//------------------------------
#include "GameElements/Troops/Troop.hpp"        // Base class for all troop types

/**
 * @brief Specialized Troop representing a Hero unit.
 *
 * Heroes are the most powerful troop, with high strength, cost, and upkeep.
 */
class Hero : public Troop {
public:
    /**
     * @brief Safely cast a generic GameElement to a Hero.
     * @param obj Weak pointer to the GameElement.
     * @return Shared pointer to Hero, or nullptr if the cast fails.
     */
    static std::shared_ptr<Hero> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a generic GameElement is actually a Hero.
     * @param obj Weak pointer to the GameElement.
     * @return true if obj wraps a Hero instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /** 
     * @brief Load shared resources for all Hero instances (sprite image).
     * Must be called once before creating any Hero.
     */
    static void init();

    /**
     * @brief Release shared resources for Hero.
     * Call this at program shutdown to free the sprite.
     */
    static void quit();

    /**
     * @brief Construct a Hero at a given position.
     * @param pos Center point in pixel coordinates.
     */
    explicit Hero(const Point& pos);

    /**
     * @brief Render the hero sprite onto the given BlitTarget.
     * Overrides the pure virtual in Displayer.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    //--------------------------------
    // Generic Accessors
    //--------------------------------

    /**
     * @brief Get this troop’s combat strength.
     * @return Integer strength value.
     */
    const int getStrength() const override { return STRENGTH; };

    /**
     * @brief Get the purchase cost for this troop.
     * @return Integer cost value.
     */
    const int getCost()     const override { return COST; };

    /**
     * @brief Get the upkeep cost per turn for this troop.
     * @return Integer upkeep value.
     */
    const int getUpkeep()   const override { return UPKEEP; };

protected:
    /// Hero’s combat strength constant.
    static constexpr int STRENGTH = 4;
    /// Hero’s purchase cost constant.
    static constexpr int COST     = 80;
    /// Hero’s upkeep cost constant.
    static constexpr int UPKEEP   = 54;

private:
    /// Shared texture for the hero sprite.
    static std::shared_ptr<Texture> sprite_;
};

#endif // UNIT_HERO_HPP
