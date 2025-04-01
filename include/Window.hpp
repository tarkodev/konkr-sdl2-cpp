#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <SDL.h>

// To free window memory on closing
struct SDLWindowDeleter {
    void operator()(SDL_Window* sdlWindow) const {
        if (sdlWindow) SDL_DestroyWindow(sdlWindow);
    }
};
using SDLWindowPtr = std::unique_ptr<SDL_Window, SDLWindowDeleter>;

// To free renderer memory on closing
struct SDLRendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        if (renderer) SDL_DestroyRenderer(renderer);
    }
};
using SDLRendererPtr = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;

/**
 * @brief Window of Konkr game.
 */
class Window {
public:
    Window(const char* title, int width, int height);
    ~Window();
    
    SDL_Renderer* getRenderer() const { return renderer_.get(); }
    bool isInitialized() const { return initialized_; }
    
private:
    bool initialized_;
    SDLWindowPtr SDLWindow_;
    SDLRendererPtr renderer_;
};

#endif
