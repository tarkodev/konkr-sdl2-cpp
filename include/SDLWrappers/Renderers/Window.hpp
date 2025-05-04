#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include "SDL.h"
#include "SDLWrappers/Coords/Size.hpp"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Utils/Checker.hpp"

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
    Window(const char* title, const Size& size);
    ~Window();

    std::shared_ptr<SDL_Renderer> getRenderer() const { return renderer_; }
    Size getSize() const override { return size_; }
    const int getWidth() const { return size_.getWidth(); }
    const int getHeight() const { return size_.getHeight(); }
    SDL_Texture* get() const override { return nullptr; };

    void fill(const SDL_Color& color) const;
    void darken() const;

    void blit(const std::weak_ptr<Texture>& src) const override;
    void blit(const std::weak_ptr<Texture>& src, const Point& destPos) const override;
    void blit(const std::weak_ptr<Texture>& src, const Size& destSize) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& destRect) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Rect& destRect) const override;

    void refresh();
    
private:
    Size size_ = {0, 0};
    std::unique_ptr<SDL_Window, SDLWindowDeleter> SDLWindow_;
    std::shared_ptr<SDL_Renderer> renderer_;

    void blit(const std::weak_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
    void blit(const std::unique_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
};

#endif
