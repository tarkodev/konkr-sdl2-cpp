#include "Window.hpp"
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdexcept>

Window::Window(const char* title, const Size& size) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(SDL_GetError());

    // Init SDL_ttf
    if (TTF_Init() < 0)
        throw std::runtime_error(TTF_GetError());

    // Init Window
    SDLWindow_.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.getWidth(), size.getHeight(), SDL_WINDOW_SHOWN));
    if (!SDLWindow_) {
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    // Set full screen and get size
    int width, height;
    SDL_SetWindowFullscreen(SDLWindow_.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(SDLWindow_.get(), &width, &height);
    size_ = {width, height};
    

    // Init Renderer
    renderer_.reset(SDL_CreateRenderer(SDLWindow_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE), SDL_DestroyRenderer);
    if (!renderer_) {
        SDL_DestroyWindow(SDLWindow_.get());
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    calc_ = std::make_unique<Texture>(renderer_, size_);
}

Window::~Window() {
    calc_ = nullptr;
    TTF_Quit();
    SDL_Quit();
}


void Window::fill(const SDL_Color& color) const {
    calc_->fill(color);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src) const {
    calc_->blit(src);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src, const Point& destPos) const {
    calc_->blit(src, destPos);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src, const Size& destSize) const {
    calc_->blit(src, destSize);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src, const Rect& destRect) const {
    calc_->blit(src, destRect);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Point& destPos) const {
    calc_->blit(src, srcRect, destPos);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Size& destSize) const {
    calc_->blit(src, srcRect, destSize);
}

void Window::blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Rect& destRect) const {
    calc_->blit(src, srcRect, destRect);
}


void Window::refresh() {
    calc_->display();
    SDL_RenderPresent(renderer_.get());
}
