#include "SDL.h"
#include "Cursor.hpp"
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "Checker.hpp"

void Cursor::init() {
    create_system_cursor(SystemCursor::Arrow, SDL_SYSTEM_CURSOR_ARROW);
    create_system_cursor(SystemCursor::Hand, SDL_SYSTEM_CURSOR_HAND);
    create_system_cursor(SystemCursor::Text, SDL_SYSTEM_CURSOR_WAIT);
}

void Cursor::quit() {
    for (auto& [type, cursor] : cursors_) {
        SDL_FreeCursor(cursor);
        SDL_Check(0, "SDL_FreeCursor");
    }
    cursors_.clear();
}


void Cursor::set(SDL_Cursor* cursor) {
    SDL_SetCursor(cursor);
    SDL_Check(0, "SDL_SetCursor");
}

void Cursor::arrow() {
    activate(SystemCursor::Arrow);
}

void Cursor::hand() {
    activate(SystemCursor::Hand);
}

void Cursor::text() {
    activate(SystemCursor::Text);
}


void Cursor::create_system_cursor(SystemCursor type, SDL_SystemCursor sdl_type) {
    SDL_Cursor* cursor = SDL_CreateSystemCursor(sdl_type);
    SDL_Check(!cursor, "Failed to create system cursor");

    cursors_[type] = cursor;
}

void Cursor::activate(SystemCursor type) {
    auto it = cursors_.find(type);
    if (it == cursors_.end())
        throw std::logic_error("Cursor not initialized");
    
    SDL_SetCursor(it->second);
    SDL_Check(0, "SDL_SetCursor");
}
