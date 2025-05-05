#ifndef UNIT_PIKEMAN_HPP
#define UNIT_PIKEMAN_HPP

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"  // Interface for render targets that can be blitted onto

//------------------------------
// Game Logic
//------------------------------
#include "GameElements/Troops/Troop.hpp"         // Base class for all troop units

/**
 * @brief A medium-tier infantry unit equipped with a pike.
 *
 * Inherits from Troop and manages a shared sprite resource.
 */
class Pikeman : public Troop {
public:
    /**
     * @brief Safely cast a generic GameElement to a Pikeman.
     * @param obj Weak pointer to the GameElement.
     * @return Shared pointer to Pikeman, or nullptr if the cast fails.
     */
    static std::shared_ptr<Pikeman> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check whether a generic GameElement is actually a Pikeman.
     * @param obj Weak pointer to the GameElement.
     * @return true if obj wraps a Pikeman instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    std::shared_ptr<GameElement> deepCopy() const override;
    
    /**
     * @brief Load the shared Pikeman sprite.
     * Must be called once before creating any Pikeman instances.
     */
    static void init();

    /**
     * @brief Release the shared Pikeman sprite.
     * Call at program shutdown to free resources.
     */
    static void quit();

    /**
     * @brief Construct a Pikeman at the specified position.
     * @param pos Center point in pixel coordinates.
     */
    explicit Pikeman(const Point& pos);

    /**
     * @brief Render the Pikeman sprite onto the given BlitTarget.
     * Overrides the pure virtual method from Displayer.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /* --- Generic Accessors --- */
    ///@{
    const int getStrength() const override { return STRENGTH; };
    const int getCost()     const override { return COST;     };
    const int getUpkeep()   const override { return UPKEEP;   };
    ///@}

protected:
    /// Base combat strength of the Pikeman.
    static constexpr int STRENGTH = 2;
    /// Recruitment cost.
    static constexpr int COST     = 20;
    /// Maintenance cost per turn.
    static constexpr int UPKEEP   = 6;

private:
    /// Shared texture resource for all Pikeman instances.
    static std::shared_ptr<Texture> sprite_;
};

#endif // UNIT_PIKEMAN_HPP
