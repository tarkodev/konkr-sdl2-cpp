#ifndef UNIT_CASTLE_HPP
#define UNIT_CASTLE_HPP

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"  // Interface for anything that can be blitted onto

//------------------------------
// Game Logic Base
//------------------------------
#include "GameElements/GameElement.hpp"          // Base class for all logical game elements

/**
 * @brief Represents a castle building in the game.
 *
 * Inherits from GameElement, providing shared logic and rendering hooks.
 */
class Castle : public GameElement {
public:
    /**
     * @brief Load the shared castle sprite.
     * Must be called once before any Castle instances are created.
     */
    static void init();

    /**
     * @brief Release the shared castle sprite.
     * Call at shutdown to free resources.
     */
    static void quit();

    /**
     * @brief Safely cast a generic GameElement to a Castle.
     * @param obj Weak pointer to a GameElement.
     * @return Shared pointer to Castle or nullptr if the cast fails.
     */
    static std::shared_ptr<Castle> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a generic GameElement is actually a Castle.
     * @param obj Weak pointer to a GameElement.
     * @return true if obj wraps a Castle instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    std::shared_ptr<GameElement> deepCopy() const override;

    /**
     * @brief Construct a Castle at a given position.
     * @param pos Center point in pixel coordinates.
     */
    explicit Castle(const Point& pos);

    /**
     * @brief Render the castle sprite onto the given BlitTarget.
     * Overrides the pure virtual in Displayer.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    //--- Generic accessors for game logic (strength, cost, upkeep) ---
    const int getStrength() const override { return STRENGTH; }
    const int getCost()     const override { return COST;     }
    const int getUpkeep()   const override { return UPKEEP;   }

protected:
    /// Base combat strength of a castle
    static constexpr int STRENGTH = 2;
    /// Build cost (in resources) of a castle
    static constexpr int COST     = 0;
    /// Maintenance cost (per turn) of a castle
    static constexpr int UPKEEP   = 0;

private:
    /// Shared texture for all Castle instances
    static std::shared_ptr<Texture> sprite_;
};

#endif // UNIT_CASTLE_HPP
