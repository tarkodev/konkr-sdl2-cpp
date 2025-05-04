#ifndef GENERICDISPLAYER_HPP
#define GENERICDISPLAYER_HPP

//------------------------------
// Coordinate Utilities
//------------------------------
#include "SDLWrappers/Coords/Point.hpp"       // Encapsulates an SDL_Point with OO interface

//------------------------------
// Base Display Interface
//------------------------------
#include "Displayers/Displayer.hpp"           // Abstract base for renderable objects

//------------------------------
// Rendering Target Abstraction
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp"// Interface for objects that can be drawn onto

/**
 * @brief A helper base class for displayers that need a fixed size.
 *
 * Inherits from Displayer to get position/size functionality,
 * and adds an overload of display() that accepts a position parameter.
 */
class GenericDisplayer : public Displayer {
public:
    /**
     * @brief Construct with a given size.
     * @param size The width/height this displayer will occupy.
     * Initializes base Displayer at pos (0,0).
     */
    GenericDisplayer(const Size& size)
      : Displayer(Point{0, 0}, size)
    {}

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~GenericDisplayer() = default;
    
    /**
     * @brief Draw the object at a specific position on the provided target.
     * @param target Weak pointer to the BlitTarget (e.g., a texture or window).
     * @param pos Pixel coordinates where the object should be rendered.
     */
    virtual void display(const std::weak_ptr<BlitTarget>& target,
                         const Point& pos) const = 0;
};

#endif // GENERICDISPLAYER_HPP
