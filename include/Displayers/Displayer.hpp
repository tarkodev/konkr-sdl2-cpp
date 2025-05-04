#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

//------------------------------
// Coordinate Abstractions
//------------------------------
#include "SDLWrappers/Coords/Point.hpp"     // Point class wrapping SDL_Point
#include "SDLWrappers/Coords/Size.hpp"      // Size class wrapping width/height

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for targets that can be blitted onto
#include "SDLWrappers/Renderers/Texture.hpp"    // Encapsulates SDL_Texture with RAII

/**
 * @brief Base class for any object that can be displayed on screen.
 *
 * Manages position and size, and holds a reference to the SDL_Renderer.
 * Subclasses must implement display() to render their content onto a BlitTarget.
 */
class Displayer {
public:
    /**
     * @brief Initialize the renderer for all Displayer instances.
     * Must be called once before creating any Displayer-derived objects.
     * @param renderer Shared pointer to the SDL_Renderer.
     */
    static void init(const std::shared_ptr<SDL_Renderer>& renderer);

    /**
     * @brief Construct a Displayer at a given position and size.
     * @param pos   Initial center position in pixels (default {0,0}).
     * @param size  Initial width/height (default {0,0}).
     */
    Displayer(const Point& pos = {0, 0}, const Size& size = {0, 0});

    virtual ~Displayer() = default;

    /**
     * @brief Render this object onto the given BlitTarget.
     * @param target Weak pointer to a BlitTarget (e.g., Window or Texture).
     */
    virtual void display(const std::weak_ptr<BlitTarget>& target) const = 0;

    /**
     * @brief Get the current center position.
     * @return Point representing the center in pixel coordinates.
     */
    virtual const Point getPos() const;

    /**
     * @brief Update the center position.
     * @param pos New center position in pixels.
     */
    virtual void setPos(const Point& pos);

    /**
     * @brief Get the current size.
     * @return Size object (width, height).
     */
    virtual Size getSize() const;

    /**
     * @brief Get the width component of size.
     * @return Width in pixels.
     */
    virtual const int getWidth() const;

    /**
     * @brief Get the height component of size.
     * @return Height in pixels.
     */
    virtual const int getHeight() const;

protected:
    /// Weak reference to the shared SDL_Renderer, set by init().
    static std::weak_ptr<SDL_Renderer> renderer_;

    /// Size (width, height) of this object.
    Size size_;

    /// Center position (x, y) in pixel coordinates.
    Point pos_;
};

#endif // DISPLAYER_HPP
