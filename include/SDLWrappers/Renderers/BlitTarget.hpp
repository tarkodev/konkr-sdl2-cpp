#ifndef BLITTARGET_HPP
#define BLITTARGET_HPP

//------------------------------
// SDL2 Core Types
//------------------------------
#include "SDL.h"  // Provides SDL_Texture, SDL_Rect, SDL_Point, etc.

//------------------------------
// Coordinate Wrappers
//------------------------------
#include "SDLWrappers/Coords/Point.hpp" // 2D integer point abstraction
#include "SDLWrappers/Coords/Size.hpp"  // Width/height abstraction
#include "SDLWrappers/Coords/Rect.hpp"  // Rectangle abstraction

#include <memory> // For std::weak_ptr

// Forward declaration to avoid circular include
class Texture;

/**
 * @brief Abstract interface for any render destination.
 *
 * A BlitTarget can report its size and underlying SDL_Texture*,
 * and supports multiple overloads of `blit()` to draw Texture instances
 * at various positions, sizes, or sub-rectangles.
 */
class BlitTarget {
public:
    /**
     * @brief Default constructor.
     */
    explicit BlitTarget() = default;

    /**
     * @brief Virtual destructor to allow safe cleanup in derived classes.
     */
    virtual ~BlitTarget() = default;

    /**
     * @brief Get the size of the render target.
     * @return Size object containing width and height.
     */
    virtual Size getSize() const = 0;

    /**
     * @brief Get the raw SDL_Texture* (if any) backing this target.
     * @return Pointer to SDL_Texture, or nullptr if none.
     */
    virtual SDL_Texture* get() const = 0;

    /**
     * @brief Draw the entire source texture at the target's origin.
     * @param src Weak pointer to the source Texture.
     */
    virtual void blit(const std::weak_ptr<Texture>& src) const = 0;

    /**
     * @brief Draw the entire source texture at a specific position.
     * @param src Weak pointer to the source Texture.
     * @param destPos Point specifying the center or top-left (implementation-dependent).
     */
    virtual void blit(const std::weak_ptr<Texture>& src,
                      const Point& destPos) const = 0;

    /**
     * @brief Draw the entire source texture scaled to a given size.
     * @param src Weak pointer to the source Texture.
     * @param destSize Desired width and height.
     */
    virtual void blit(const std::weak_ptr<Texture>& src,
                      const Size& destSize) const = 0;

    /**
     * @brief Draw the entire source texture into a specified rectangle.
     * @param src Weak pointer to the source Texture.
     * @param destRect Rectangle defining destination area.
     */
    virtual void blit(const std::weak_ptr<Texture>& src,
                      const Rect& destRect) const = 0;

    /**
     * @brief Draw a sub-region of the source texture at a given position.
     * @param src      Weak pointer to the source Texture.
     * @param srcRect  Rectangle defining the portion of the source to copy.
     * @param destPos  Point specifying where to draw the sub-region.
     */
    virtual void blit(const std::weak_ptr<Texture>& src,
                      const Rect& srcRect,
                      const Point& destPos) const = 0;

    /**
     * @brief Draw a sub-region of the source texture scaled to a given size.
     * @param src      Weak pointer to the source Texture.
     * @param srcRect  Rectangle defining the portion of the source to copy.
     * @param destSize Desired width and height for the drawn portion.
     */
    virtual void blit(const std::weak_ptr<Texture>& src,
                      const Rect& srcRect,
                      const Size& destSize) const = 0;

    /**
     * @brief Draw a sub-region of the source texture into a specified rectangle.
     * @param src      Weak pointer to the source Texture.
     * @param srcRect  Portion of the source to copy.
     * @param destRect Destination rectangle where the sub-region is rendered.
     */
    virtual void blit(const std::weak_ptr<Texture>& src,
                      const Rect& srcRect,
                      const Rect& destRect) const = 0;
};

#endif // BLITTARGET_HPP
