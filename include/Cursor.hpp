#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <SDL2/SDL.h>
#include <unordered_map>

class Cursor {
public:
    // Liste des curseurs système supportés
    enum class SystemCursor {
        Arrow,
        Hand,
        Text
    };

    // Initialize default cursors
    static void init();
    static void quit();

    // Set new cursor
    static void set(SDL_Cursor* cursor);

    static void arrow();
    static void hand();
    static void text();

private:
    static inline std::unordered_map<SystemCursor, SDL_Cursor*> cursors_;

    static void create_system_cursor(SystemCursor type, SDL_SystemCursor sdl_type);
    static void activate(SystemCursor type);

    Cursor() = delete;
    ~Cursor() = delete;
};

#endif
