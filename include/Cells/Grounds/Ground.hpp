#ifndef GROUND_HPP
#define GROUND_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"  // Provides fundamental SDL types (SDL_Point, SDL_Rect, etc.)

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"   // Interface for objects that can be blitted
#include "SDLWrappers/Renderers/Texture.hpp"      // RAII wrapper around SDL_Texture
#include "Displayers/Displayer.hpp"               // Base class that holds position/size and renderer
#include "Displayers/HexagonDisplayer.hpp"        // Specialized displayer for hexagonal tiles

//------------------------------
// Game Logic
//------------------------------
#include "GameElements/Player.hpp" // Needed if ground ownership by players is ever relevant
#include "Cells/Cell.hpp"          // Base class for all grid cells
#include <memory>                  // std::shared_ptr, std::weak_ptr

/**
 * @brief Represents a generic terrain hexagon ("island") on the map.
 *
 * Inherits both Cell (grid logic) and Displayer (rendering logic).
 * Uses a shared HexagonDisplayer to draw the hexagon shape and its neighbor links.
 */
class Ground : public Cell, public Displayer {
public:
    /**
     * @brief Safely cast a generic Cell to a Ground.
     * @param obj Weak pointer to the base Cell.
     * @return Shared pointer to Ground or nullptr if the cast fails.
     */
    static std::shared_ptr<Ground> cast(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Check if a generic Cell instance is actually a Ground.
     * @param obj Weak pointer to the base Cell.
     * @return true if obj wraps a Ground.
     */
    static const bool is(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Initialize the shared hexagon textures and metrics.
     * Must be called once before any Ground instances are created.
     */
    static void init();

    /**
     * @brief Release the shared resources (textures) used by Ground.
     * Call this at program shutdown.
     */
    static void quit();

    /**
     * @brief Get the size (width × height) of the hexagon texture.
     * @return Size object representing the pixel dimensions.
     */
    static const Size getIslandSize();

    /**
     * @brief Get the outer radius (distance from center to vertex) of the hexagon.
     * @return Radius in pixels.
     */
    static const double getRadius();

    /**
     * @brief Get the inner radius (distance from center to midpoint of a side).
     * @return Inner radius in pixels.
     */
    static const double getInnerRadius();

    /**
     * @brief Render the hexagon onto the given target, drawing neighbor links as needed.
     * Overrides the pure virtual method from Displayer.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

protected:
    /**
     * @brief Construct a Ground at a specific position.
     * @param pos Center point (in pixels) where this hexagon will be drawn.
     */
    explicit Ground(const Point& pos);

    /// Shared displayer responsible for drawing the hexagon shape and neighbor links.
    static HexagonDisplayer islandDisplayer_;

    /// Distance from center to vertex of the hexagon texture.
    static double islandRadius_;

    /// Distance from center to midpoint of a side of the hexagon texture.
    static double islandInnerRadius_;
};

#endif // GROUND_HPP
