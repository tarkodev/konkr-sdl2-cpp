#ifndef DRAWUTILS_HPP
#define DRAWUTILS_HPP

#include <SDL.h>

inline void SetRenderDrawColor(SDL_Renderer* renderer, const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#endif
