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
#include "Widgets/GameMap.hpp"   // Interactive game map widget
#include "Widgets/Button.hpp"    // Clickable button widget
#include "GameElements/Player.hpp"    // Clickable button widget

#include <memory>
#include <string>

/** 
 * @brief Forward declaration for the main submenu.
 */
class MainMenu;

class MakeMenu : public MenuBase {
public:
    /**
     * @brief Create a MakeMenu instance.
     * @param window Shared pointer to the application window/renderer.
     * @param mapPath Path to the map configuration file.
     */
    MakeMenu(const std::shared_ptr<Window>& window);

    /** @brief Default destructor. */
    ~MakeMenu() = default;

    /**
     * @brief Enter the menu's main loop until exit.
     * @return Next MenuBase to transition to (or nullptr to quit).
     */
    std::shared_ptr<MenuBase> run() override;

protected:
    /**
     * @brief Handle all pending SDL events.
     *
     * Invokes specialized callbacks for mouse, keyboard.
     */
    void handleEvents() override;

    /**
     * @brief Draw all widgets (map, buttons) to the screen.
     */
    void draw() override;

private:
    // Widgets and textures
    std::unique_ptr<GameMap>      map_;         ///< Interactive hex map
    std::unique_ptr<Button>       backBtn_;     ///< Button to go back to main menu
    std::vector<std::shared_ptr<Player>> players_;

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

    // Initialization helpers
    void createMap(const std::string& mapPath);  ///< Load and instantiate the GameMap
    void updateMapPos();                         ///< Recalculate mapPos_ on resize/zoom
    void updateMapPosAndSize();

    // Event-specific handlers
    void onMouseButtonDown(SDL_Event& event);
    void onMouseMotion(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);
    void onMouseWheel(SDL_Event& event);
    void onKeyDown(SDL_Event& event);
};

#endif // GAMEMENU_HPP
