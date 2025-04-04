#include "Window.hpp"
#include <SDL.h>
#include <stdexcept>

Window::Window(const char* title, int width, int height) : initialized_(false) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(SDL_GetError());

    // Init Window
    SDLWindow_.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));
    if (!SDLWindow_) {
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    // Set full screen and get size
    SDL_SetWindowFullscreen(SDLWindow_.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(SDLWindow_.get(), &size_.w, &size_.h);

    // Init Renderer
    renderer_.reset(SDL_CreateRenderer(SDLWindow_.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!renderer_) {
        SDL_DestroyWindow(SDLWindow_.get());
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    // Initialization success
    initialized_ = true;
}

Window::~Window() {
    SDL_Quit();
}
