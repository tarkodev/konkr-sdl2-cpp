#ifndef FOREST_HPP
#define FOREST_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"  // Provides SDL_Point, SDL_Renderer, SDL_Color, etc.

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"    // Encapsulates SDL_Texture with RAII
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for anything that can be blitted onto

//------------------------------
// Game Logic
//------------------------------
#include "GameElements/Player.hpp"          // Forward declares Player for ownership logic
#include "Cells/Grounds/PlayableGround.hpp" // PlayableGround base class for cell behaviors

/**
 * @brief A terrain cell representing a forest.
 *
 * Inherits from Ground (which itself is a Displayer + Cell).
 * Forests are static obstacles that display a tree texture.
 */
class Forest : public Ground {
public:
    /**
     * @brief Safely cast a generic Cell to a Forest.
     * @param obj Weak pointer to the cell.
     * @return Shared pointer to Forest or nullptr if the cast fails.
     */
    static std::shared_ptr<Forest> cast(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Check if a generic Cell is actually a Forest.
     * @param obj Weak pointer to the cell.
     * @return true if obj wraps a Forest instance.
     */
    static const bool is(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Load the shared forest texture.
     * Must be called once before any Forest instances are created.
     */
    static void init();

    /**
     * @brief Release the shared forest texture.
     * Call this at program shutdown to free resources.
     */
    static void quit();

    /**
     * @brief Construct a Forest at a given position.
     * @param pos Center point in pixel coordinates.
     */
    explicit Forest(const Point& pos);

    /**
     * @brief Render the forest texture onto the given BlitTarget.
     * Overrides the pure virtual in Displayer.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

protected:
    /// Shared texture for all Forest instances (tree image).
    static std::shared_ptr<Texture> forest_;
};

#endif // FOREST_HPP
