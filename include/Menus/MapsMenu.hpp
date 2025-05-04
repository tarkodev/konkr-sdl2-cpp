#ifndef MAPSMENU_HPP
#define MAPSMENU_HPP

//------------------------------
// Base Menu Abstraction
//------------------------------
#include "Menus/MenuBase.hpp"  // Defines MenuBase, the common interface for all menus

//------------------------------
// Forward Declarations
//------------------------------
class MainMenu;  // Forward declare MainMenu for return-to-main logic

/**
 * @brief Menu that lists available game maps.
 *
 * Inherits from MenuBase to provide a simple button-based interface
 * where each button loads a different map. Also includes a "Back" button
 * to return to the MainMenu.
 */
class MapsMenu : public MenuBase {
public:
    /**
     * @brief Construct the MapsMenu.
     * @param window Shared pointer to the Window for rendering.
     */
    explicit MapsMenu(const std::shared_ptr<Window>& window);

    /**
     * @brief Default destructor.
     */
    ~MapsMenu() override = default;

    /**
     * @brief Main loop of the menu.
     * @return Next MenuBase to run (either GameMenu or MainMenu).
     */
    std::shared_ptr<MenuBase> run() override;

protected:
    /**
     * @brief Handle SDL events (mouse, keyboard) for map selection and Back button.
     */
    void handleEvents() override;

    /**
     * @brief Render all buttons onto the window.
     */
    void draw() override;

private:
    /// Collection of buttons representing each map and the Back option
    std::vector<Button> buttons_;
};

#endif // MAPSMENU_HPP
