// Button.hpp
#ifndef BUTTON_HPP
#define BUTTON_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"  // SDL_Event, SDL_MOUSEBUTTONDOWN, SDL_MOUSEMOTION, etc.

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"    // Encapsulates SDL_Texture for blitting
#include "Displayers/Displayer.hpp"             // Base class providing position & size
#include "SDLWrappers/Coords/Rect.hpp"          // Defines Rect for collision/hit testing
#include "SDLWrappers/Coords/Point.hpp"         // Defines Point for 2D coordinates
#include "SDLWrappers/Coords/Size.hpp"          // Defines Size for width/height

//------------------------------
// Standard Library
//------------------------------
#include <functional>  // std::function for callbacks
#include <memory>      // std::shared_ptr, std::weak_ptr, etc.

/**
 * @brief A clickable UI button with normal, hover, and pressed states.
 *
 * Inherits from Displayer to have position/size and implements event handling
 * to manage hover/press visuals and invoke callbacks.
 */
class Button : public Displayer {
public:
    /// Signature for click or press callbacks.
    using Callback = std::function<void()>;

    /**
     * @brief Construct a Button at a given position with image paths.
     * @param pos      Center position of the button.
     * @param normal   Filepath for the default button image.
     * @param hover    Filepath for the hover state image (optional).
     * @param pressed  Filepath for the pressed state image (optional).
     */
    Button(const Point& pos,
           const std::string& normal,
           const std::string& hover = "",
           const std::string& pressed = "");

    /// Default destructor.
    ~Button() = default;

    /// Non-copyable to avoid duplicating SDL resources.
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
  
    /// Movable to allow storing in containers.
    Button(Button&&) noexcept = default;
    Button& operator=(Button&&) noexcept = default;

    /**
     * @brief Query whether the mouse is currently over the button.
     * @param mousePos Current mouse position.
     * @return true if mouse is within button bounds.
     */
    const bool isHover(const Point& mousePos) const;

    /**
     * @brief Process SDL events to update hover/press state and invoke callbacks.
     * @param e SDL_Event from the event loop.
     */
    void handleEvent(const SDL_Event& e);

    /**
     * @brief Set callback to invoke when a press is released inside the button.
     * @param cb Function to call on click.
     */
    void setCallback(Callback cb);

    /**
     * @brief Set callback to invoke immediately on mouse-down inside the button.
     * @param cb Function to call on press.
     */
    void setPressedCallback(Callback cb);

    /**
     * @brief Render the appropriate sprite based on hover/pressed state.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

private:
    /// Shared pointer to the normal-state texture.
    std::shared_ptr<Texture> sprite_;
    /// Shared pointer to the hover-state texture (if provided).
    std::shared_ptr<Texture> hoverSprite_;
    /// Shared pointer to the pressed-state texture (if provided).
    std::shared_ptr<Texture> pressedSprite_;

    /// Callback invoked on mouse-down inside the button.
    Callback pressedCallback_ = nullptr;
    /// Callback invoked on mouse-up (click) inside the button.
    Callback callback_ = nullptr;
    /// Internal flag tracking whether the cursor is hovering.
    bool     isHover_ = false;
    /// Internal flag tracking whether the button is currently pressed.
    bool     isPressed_ = false;
};

#endif // BUTTON_HPP
