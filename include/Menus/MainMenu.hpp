#ifndef MAINMENU_HPP
#define MAINMENU_HPP

//------------------------------
// Coordinate Utilities
//------------------------------
#include "SDLWrappers/Coords/Point.hpp"           // Represents 2D points and simple vector arithmetic

//------------------------------
// Base Menu Abstraction
//------------------------------
#include "Menus/MenuBase.hpp"                     // Defines the abstract MenuBase interface

//------------------------------
// Rendering
//------------------------------
#include "SDLWrappers/Renderers/Window.hpp"       // Provides Window class for rendering context
#include "SDLWrappers/Renderers/Texture.hpp"      // Encapsulates SDL textures for images

//------------------------------
// UI Elements
//------------------------------
#include "Widgets/Button.hpp"                     // Clickable button with hover/pressed states

#include <memory>                                  // std::shared_ptr, std::unique_ptr

// Forward declaration to avoid circular include
class MapsMenu;

/**
 * @brief Main title screen menu of the application.
 *
 * Displays background, logo, and three buttons:
 *  - Start Expedition (go to MapsMenu)
 *  - How to Play (open external tutorial)
 *  - Exit (quit application)
 *
 * Inherits from MenuBase to implement run(), event handling, and drawing.
 */
class MainMenu : public MenuBase {
public:
    /**
     * @brief Construct the main menu with a reference to the render window.
     * @param window Shared pointer to the application's Window.
     */
    MainMenu(const std::shared_ptr<Window>& window);

    /// Default destructor
    ~MainMenu() = default;

    /**
     * @brief Execute the menu loop until an action transitions out.
     * @return Next MenuBase state (nullptr to exit).
     */
    std::shared_ptr<MenuBase> run() override;

protected:
    /**
     * @brief Poll and dispatch SDL events to UI elements.
     * Overrides MenuBase.
     */
    void handleEvents() override;

    /**
     * @brief Render background, logo, and buttons each frame.
     * Overrides MenuBase.
     */
    void draw() override;

private:
    /// Background image texture
    std::shared_ptr<Texture> bg_;

    /// Game logo texture
    std::shared_ptr<Texture> logo_;

    /// Button to start the expedition (enter MapsMenu)
    std::unique_ptr<Button> expeditionBtn_;

    /// Button to open the "How to Play" web page
    std::unique_ptr<Button> howToPlayBtn_;

    /// Button to exit the application
    std::unique_ptr<Button> exitBtn_;

    /// Screen position to draw the logo
    Point logoPos_;
};

#endif // MAINMENU_HPP
