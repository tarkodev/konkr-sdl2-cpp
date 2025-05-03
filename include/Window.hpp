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
    Window(const char* title, const Size& size);
    ~Window();

    std::shared_ptr<SDL_Renderer> getRenderer() const { return renderer_; }
    Size getSize() const override { return size_; }
    int getWidth() const { return size_.getWidth(); }
    int getHeight() const { return size_.getHeight(); }
    SDL_Texture* get() const override { return calc_->get(); };

    void fill(const SDL_Color& color) const;

    void blit(const std::weak_ptr<BlitTarget>& src) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Point& destPos) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Size& destSize) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& destRect) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Rect& destRect) const override;

    void refresh();
    
private:
    std::unique_ptr<Texture> calc_;

    Size size_ = {0, 0};
    std::unique_ptr<SDL_Window, SDLWindowDeleter> SDLWindow_;
    std::shared_ptr<SDL_Renderer> renderer_;
};

#endif
