#ifndef UNIT_KNIGHT_HPP
#define UNIT_KNIGHT_HPP

//------------------------------
// Rendering Abstraction
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for anything that can be drawn

//------------------------------
// Game Logic
//------------------------------
#include "GameElements/Troops/Troop.hpp"       // Base class for all troop units

/**
 * @brief Represents a Knight unit on the game map.
 *
 * Inherits from Troop, providing movement and combat behavior,
 * and renders its specific knight sprite.
 */
class Knight : public Troop {
public:
    /**
     * @brief Safely cast a generic GameElement to a Knight.
     * @param obj Weak pointer to a GameElement.
     * @return Shared pointer to Knight, or nullptr if the cast fails.
     */
    static std::shared_ptr<Knight> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check whether a GameElement is actually a Knight.
     * @param obj Weak pointer to a GameElement.
     * @return true if obj wraps a Knight instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    std::shared_ptr<GameElement> deepCopy() const override;

    /**
     * @brief Load shared resources (e.g. sprite texture) for all Knights.
     * Must be called once before creating Knight instances.
     */
    static void init();

    /**
     * @brief Release shared Knight resources at shutdown.
     */
    static void quit();

    /**
     * @brief Construct a Knight at the specified position.
     * @param pos Center position in pixel coordinates.
     */
    explicit Knight(const Point& pos);

    /**
     * @brief Render the knight sprite onto the given BlitTarget.
     * @param target Weak pointer to the rendering target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /* --- Generic Accessors --- */

    /// Combat strength value of this unit.
    const int getStrength() const override { return STRENGTH; };

    /// Production cost to recruit this unit.
    const int getCost()     const override { return COST; };

    /// Upkeep cost per turn for this unit.
    const int getUpkeep()   const override { return UPKEEP; };

protected:
    /// Base combat power.
    static constexpr int STRENGTH = 3;
    /// Resource cost to create.
    static constexpr int COST     = 40;
    /// Recurring upkeep cost per turn.
    static constexpr int UPKEEP   = 18;

private:
    /// Shared sprite texture for all Knight instances.
    static std::shared_ptr<Texture> sprite_;
};

#endif // UNIT_KNIGHT_HPP
