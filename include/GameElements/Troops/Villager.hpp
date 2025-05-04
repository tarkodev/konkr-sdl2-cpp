#ifndef UNIT_VILLAGER_HPP
#define UNIT_VILLAGER_HPP

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for objects that can be blitted (drawn) onto a render target

//------------------------------
// Game Logic Base Class
//------------------------------
#include "GameElements/Troops/Troop.hpp"      // Base class for all troop units, provides common behavior

/**
 * @brief A basic civilian unit that can move and be recruited.
 *
 * Villagers have low strength, cost, and upkeep.
 * Inherits common display and logic behavior from Troop.
 */
class Villager : public Troop {
public:
    /**
     * @brief Safely cast a generic GameElement to a Villager.
     * @param obj Weak pointer to the GameElement.
     * @return Shared pointer to Villager or nullptr if the cast fails.
     */
    static std::shared_ptr<Villager> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a generic GameElement is actually a Villager.
     * @param obj Weak pointer to the GameElement.
     * @return true if obj wraps a Villager instance.
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);
    
    /**
     * @brief Load the shared villager sprite.
     * Must be called once before creating any Villager instances.
     */
    static void init();

    /**
     * @brief Release the shared villager sprite.
     * Call at program shutdown to free resources.
     */
    static void quit();

    /**
     * @brief Construct a Villager at the given map position.
     * @param pos Center point in pixel coordinates.
     */
    explicit Villager(const Point& pos);

    /**
     * @brief Render this villager onto the given render target.
     * Overrides the pure virtual in Displayer via Troop → GameElement → Displayer.
     * @param target Weak pointer to a BlitTarget to draw onto.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    //--- Generic stat accessors ------------------------------------------------

    /**
     * @brief Strength rating (combat power).
     * @return Strength value for a Villager.
     */
    const int getStrength() const override { return STRENGTH; };

    /**
     * @brief Recruitment cost in treasury units.
     * @return Cost to recruit a Villager.
     */
    const int getCost()     const override { return COST; };

    /**
     * @brief Upkeep (maintenance) cost per turn.
     * @return Upkeep for a Villager.
     */
    const int getUpkeep()   const override { return UPKEEP; };

protected:
    /// Combat power of a Villager
    static constexpr int STRENGTH = 1;
    /// Recruitment cost for a Villager
    static constexpr int COST     = 10;
    /// Upkeep per turn for a Villager
    static constexpr int UPKEEP   = 2;

private:
    /// Shared texture for all Villager instances
    static std::shared_ptr<Texture> sprite_;
};

#endif // UNIT_VILLAGER_HPP
