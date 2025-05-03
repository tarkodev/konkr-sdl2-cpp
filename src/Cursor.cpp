#include "SDL.h"
#include "Cursor.hpp"
#include <stdexcept>
#include <unordered_map>
#include <string>

void Cursor::init() {
    create_system_cursor(SystemCursor::Arrow, SDL_SYSTEM_CURSOR_ARROW);
    create_system_cursor(SystemCursor::Hand, SDL_SYSTEM_CURSOR_HAND);
    create_system_cursor(SystemCursor::Text, SDL_SYSTEM_CURSOR_WAIT);
}

void Cursor::quit() {
    for (auto& [type, cursor] : cursors_)
        SDL_FreeCursor(cursor);
    cursors_.clear();
}


void Cursor::set(SDL_Cursor* cursor) {
    SDL_SetCursor(cursor);
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
    if (!cursor)
        throw std::runtime_error("Failed to create system cursor: " + std::string(SDL_GetError()));

    cursors_[type] = cursor;
}

void Cursor::activate(SystemCursor type) {
    auto it = cursors_.find(type);
    if (it == cursors_.end())
        throw std::logic_error("Cursor not initialized");
    
    SDL_SetCursor(it->second);
}
