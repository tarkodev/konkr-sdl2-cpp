#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

//------------------------------
// Geometry & Coordinates
//------------------------------
#include "SDLWrappers/Coords/Point.hpp"    // 2D point abstraction

//------------------------------
// Menu Framework
//------------------------------
#include "Menus/MenuBase.hpp"              // Base class for all menus

//------------------------------
// Rendering
//------------------------------
#include "SDLWrappers/Renderers/Window.hpp"  // Encapsulates SDL_Window and SDL_Renderer
#include "SDLWrappers/Renderers/Texture.hpp" // RAII wrapper around SDL_Texture

//------------------------------
// UI Widgets
//------------------------------
#include "Widgets/Overlay.hpp"   // Semi-transparent overlay widget
#include "Widgets/GameMap.hpp"   // Interactive game map widget
#include "Widgets/Button.hpp"    // Clickable button widget

#include <memory>
#include <string>

/** 
 * @brief Forward declaration for the maps submenu.
 */
class MapsMenu;

/**
 * @brief In-game menu that displays the map, overlay, and controls.
 *
 * Inherits from MenuBase to implement the main game interaction loop:
 *   - Processes input events
 *   - Updates map and UI positions
 *   - Renders widgets to the window
 */
class GameMenu : public MenuBase {
public:
    /**
     * @brief Create a GameMenu instance.
     * @param window Shared pointer to the application window/renderer.
     * @param mapPath Path to the map configuration file.
     */
    GameMenu(const std::shared_ptr<Window>& window, const std::string& mapPath);

    /** @brief Default destructor. */
    ~GameMenu() = default;

    /**
     * @brief Enter the menu's main loop until exit.
     * @return Next MenuBase to transition to (or nullptr to quit).
     */
    std::shared_ptr<MenuBase> run() override;

protected:
    /**
     * @brief Handle all pending SDL events.
     *
     * Invokes specialized callbacks for mouse, keyboard, and overlay.
     */
    void handleEvents() override;

    /**
     * @brief Draw all widgets (map, overlay, buttons) to the screen.
     */
    void draw() override;

private:
    // Widgets and textures
    std::unique_ptr<GameMap>      map_;         ///< Interactive hex map
    std::unique_ptr<Overlay>      overlay_;     ///< GUI overlay (e.g., stats panel)
    std::unique_ptr<Button>       backBtn_;     ///< Button to go back to main menu
    std::unique_ptr<Button>       finishBtn_;   ///< "Finish Turn" button
    std::shared_ptr<Texture>      finishTex_;   ///< Texture for the finish button

    // Window dimensions (pixels)
    Size windowSize_;                       

    // Hex cell sizing constraints
    Size minHexSize_;                      
    Size maxHexSize_;                      

    // Map sizing & position
    Size minMapSize_;                      
    Size maxMapSize_;                      
    Size mapCellSize_;                     
    Point mapPos_;                         

    // Frame tracking for performance/logging
    int     frameCount_;                   
    Uint32  lastLogTime_;                  

    // Drag-and-drop state for map panning
    std::optional<Point> moveOrigin_;      ///< Where the drag started
    bool moved_            = false;        ///< Whether a drag occurred

    // Game state
    bool gameFinished_     = false;        ///< Has the player finished their turn?

    // Initialization helpers
    void createMap(const std::string& mapPath);  ///< Load and instantiate the GameMap
    void updateMapPos();                         ///< Recalculate mapPos_ on resize/zoom
    void updateShop();                           ///< Refresh shop items if implemented

    // Event-specific handlers
    void onMouseButtonDown(SDL_Event& event);
    void onMouseMotion(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);
    void onMouseWheel(SDL_Event& event);
    void onKeyDown(SDL_Event& event);

    /**
     * @brief Delegate events to the overlay widget.
     * @param event SDL event to forward.
     * @return true if overlay consumed the event (stop further handling).
     */
    const bool handleOverlay(SDL_Event& event);
};

#endif // GAMEMENU_HPP
