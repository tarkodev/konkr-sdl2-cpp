#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include "SDL.h"
#include "Size.hpp"
#include "BlitTarget.hpp"
#include "Texture.hpp"

// To free window memory on closing
struct SDLWindowDeleter {
    void operator()(SDL_Window* sdlWindow) const {
        if (sdlWindow) SDL_DestroyWindow(sdlWindow);
    }
};

/**
 * @brief Window of Konkr game.
 */
class Window: public BlitTarget {
public:
    Window(const char* title, int width, int height);
    ~Window();
    
    bool isInitialized() const { return initialized_; }

    SDL_Renderer* getRenderer() const { return renderer_.get(); }
    Size getSize() const override { return size_; }
    SDL_Texture* get() const override { return calc_->get(); };

    void fill(const SDL_Color& color) const;

    void blit(const BlitTarget* src) const override;
    void blit(const BlitTarget* src, const Point& destPos) const override;
    void blit(const BlitTarget* src, const Size& destSize) const override;
    void blit(const BlitTarget* src, const Rect& destRect) const override;
    void blit(const BlitTarget* src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const BlitTarget* src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const BlitTarget* src, const Rect& srcRect, const Rect& destRect) const override;

    void refresh();
    
private:
    bool initialized_;
    std::unique_ptr<Texture> calc_;

    Size size_ = {0, 0};
    std::unique_ptr<SDL_Window, SDLWindowDeleter> SDLWindow_;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer_; //! encapsuler le renderer dans une classe
};

#endif
