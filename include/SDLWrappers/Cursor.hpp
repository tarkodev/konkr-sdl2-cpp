#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "SDLWrappers/Coords/Point.hpp"

#include "SDL.h"
#include <unordered_map>

class Cursor {
public:
    // Liste des curseurs système supportés
    enum class SystemCursor {
        Any,
        Arrow,
        Text,
        Hand
    };

    // Initialize default cursors
    static void init();
    static void quit();

    // Set new cursor
    static void set(SDL_Cursor* cursor);

    static void requestArrow();
    static void requestText();
    static void requestHand();
    static void update();

    static const Point getPos();

private:
    static inline std::unordered_map<SystemCursor, SDL_Cursor*> cursors_;
    static inline SystemCursor requestedCursor_ = SystemCursor::Any;

    static void create_system_cursor(SystemCursor type, SDL_SystemCursor sdl_type);
    static void activate(SystemCursor type);

    Cursor() = delete;
    ~Cursor() = delete;
};

#endif
