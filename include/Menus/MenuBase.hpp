#ifndef MENUBASE_HPP
#define MENUBASE_HPP

//------------------------------
// Rendering Abstraction
//------------------------------
#include "Displayers/Displayer.hpp"         // Base class for objects that can render themselves

//------------------------------
// Window Management
//------------------------------
#include "SDLWrappers/Renderers/Window.hpp" // SDL-backed window and renderer abstraction

//------------------------------
// UI Components
//------------------------------
#include "Widgets/Button.hpp"               // Clickable button widget

//------------------------------
// Standard Library
//------------------------------
#include <vector>                            // std::vector for dynamic collections
#include <memory>                            // std::shared_ptr for shared ownership

/**
 * @brief Abstract base for all in-game menus.
 *
 * Manages the main event/render loop, holds a reference to the Window,
 * and coordinates transitions to other menus.
 */
class MenuBase {
public:
    /**
     * @brief Construct a MenuBase tied to a given Window.
     * @param window Shared pointer to the application's Window.
     */
    MenuBase(const std::shared_ptr<Window>& window)
        : window_(window) {}

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~MenuBase() = default;
    
    /**
     * @brief Execute the menu's loop until it finishes.
     * @return Shared pointer to the next menu, or nullptr to quit.
     */
    virtual std::shared_ptr<MenuBase> run() = 0;

protected:
    /// The Window to render into and poll events from.
    std::shared_ptr<Window> window_;
    /// Controls the menu loop; true to continue, false to exit.
    bool loop_ = true;
    /// Next menu to transition to after this one ends.
    std::shared_ptr<MenuBase> nextMenu_;

    /**
     * @brief Draw all menu elements.
     * Must be implemented by derived classes.
     */
    virtual void draw() = 0;

    /**
     * @brief Handle all pending events for this menu.
     * Must be implemented by derived classes.
     */
    virtual void handleEvents() = 0;

    /**
     * @brief Handle a single SDL event (e.g., SDL_QUIT).
     * @param e The event to process.
     */
    virtual void handleEvent(const SDL_Event& e);
};

#endif // MENUBASE_HPP
