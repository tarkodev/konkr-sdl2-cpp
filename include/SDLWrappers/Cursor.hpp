#ifndef CURSOR_HPP
#define CURSOR_HPP

//------------------------------
// Coordinate Abstraction
//------------------------------
#include "SDLWrappers/Coords/Point.hpp" // Provides Point class for 2D positions

//------------------------------
// SDL Core
//------------------------------
#include "SDL.h"                // SDL_CreateSystemCursor, SDL_SetCursor, SDL_GetMouseState, etc.
#include <unordered_map>        // std::unordered_map for mapping enum to SDL_Cursor*

/**
 * @brief Manages system cursors (mouse pointers).
 *
 * Provides an interface to initialize, switch, and update the cursor
 * based on UI interactions (hover, click, text entry, etc.).
 */
class Cursor {
public:
    /**
     * @brief Supported system cursor types.
     *
     * - Any:    No preference; leave current.
     * - Arrow:  Standard arrow pointer.
     * - Text:   Text-input I-beam cursor.
     * - Hand:   Hand pointer for clickable elements.
     */
    enum class SystemCursor {
        Any,
        Arrow,
        Text,
        Hand
    };

    /**
     * @brief Load and create the default system cursors.
     *
     * Must be called once after SDL initialization.
     */
    static void init();

    /**
     * @brief Free all loaded system cursors.
     *
     * Call on program shutdown to avoid leaks.
     */
    static void quit();

    /**
     * @brief Immediately set the OS cursor to the given SDL_Cursor.
     * @param cursor Raw SDL_Cursor pointer obtained from SDL_CreateSystemCursor.
     */
    static void set(SDL_Cursor* cursor);

    /**
     * @brief Request the arrow cursor on next update cycle.
     *
     * Does not apply immediately; see update().
     */
    static void requestArrow();

    /**
     * @brief Request the text (I-beam) cursor on next update cycle.
     */
    static void requestText();

    /**
     * @brief Request the hand pointer cursor on next update cycle.
     */
    static void requestHand();

    /**
     * @brief Apply any pending cursor request.
     *
     * Should be called at end of event loop to batch cursor changes.
     */
    static void update();

    /**
     * @brief Get the current mouse position.
     * @return Point containing (x, y) in window coordinates.
     */
    static const Point getPos();

private:
    /// Map each SystemCursor enum to its SDL_Cursor* handle.
    static inline std::unordered_map<SystemCursor, SDL_Cursor*> cursors_;

    /// The cursor type requested; reset to Any after each update().
    static inline SystemCursor requestedCursor_ = SystemCursor::Any;

    /**
     * @brief Internal helper: create an SDL_Cursor for given type.
     * @param type      The SystemCursor to map.
     * @param sdl_type  SDL's system cursor enum constant.
     */
    static void create_system_cursor(SystemCursor type, SDL_SystemCursor sdl_type);

    /**
     * @brief Internal helper: activate the given cursor immediately.
     * @param type The SystemCursor to activate.
     */
    static void activate(SystemCursor type);

    // Disable instantiation and destruction: all methods are static.
    Cursor() = delete;
    ~Cursor() = delete;
};

#endif // CURSOR_HPP
