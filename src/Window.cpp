#include "Window.hpp"
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include "RenderTargetGuard.hpp"
#include "Checker.hpp"

#include <stdexcept>
#include <memory>

Window::Window(const char* title, const Size& size) {
    // Init SDL
    SDL_Check(SDL_Init(SDL_INIT_VIDEO), "SDL_Init");

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
    if (SDL_SetWindowFullscreen(SDLWindow_.get(), SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
        SDL_DestroyWindow(SDLWindow_.get());
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_SetWindowFullscreen failed: ") + SDL_GetError());
    }
    SDL_GetWindowSize(SDLWindow_.get(), &width, &height);
    size_ = {width, height};
    

    // Init Renderer
    renderer_.reset(SDL_CreateRenderer(SDLWindow_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);
    if (!renderer_) {
        SDL_DestroyWindow(SDLWindow_.get());
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateRenderer") + SDL_GetError());
    }
}

Window::~Window() {
    TTF_Quit();
    SDL_Quit();
}


void Window::fill(const SDL_Color& color) const {
    RenderTargetGuard target(renderer_, std::shared_ptr<SDL_Texture>());
    SDL_Check(SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b, color.a), "SDL_SetRenderDrawColor");
    SDL_Check(SDL_RenderClear(renderer_.get()), "SDL_RenderClear");

}

void Window::blit(const std::weak_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    auto lsrc = src.lock();
    if (!lsrc) return;

    lsrc->convertAlpha();
    RenderTargetGuard target(renderer_, std::shared_ptr<SDL_Texture>());
    SDL_Check(SDL_RenderCopy(renderer_.get(), lsrc->get(), srcRect, destRect), "SDL_RenderCopy");
}

void Window::blit(const std::weak_ptr<Texture>& src) const {
    blit(src, nullptr, nullptr);
}

void Window::blit(const std::weak_ptr<Texture>& src, const Point& destPos) const {
    if (auto lsrc = src.lock()) {
        Rect destRect(destPos, lsrc->getSize());
        blit(src, nullptr, &destRect.get());
    }
}

void Window::blit(const std::weak_ptr<Texture>& src, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, nullptr, &destRect.get());
}

void Window::blit(const std::weak_ptr<Texture>& src, const Rect& destRect) const {
    blit(src, nullptr, &destRect.get());
}

void Window::blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Point& destPos) const {
    if (auto lsrc = src.lock()) {
        Rect destRect(destPos, lsrc->getSize());
        blit(src, &srcRect.get(), &destRect.get());
    }
}

void Window::blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, &srcRect.get(), &destRect.get());
}

void Window::blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Rect& destRect) const {
    blit(src, &srcRect.get(), &destRect.get());
}


void Window::blit(const std::unique_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    src->convertAlpha();
    RenderTargetGuard target(renderer_, std::shared_ptr<SDL_Texture>());
    SDL_Check(SDL_RenderCopy(renderer_.get(), src->get(), srcRect, destRect), "SDL_RenderCopy");
}


void Window::refresh() {
    RenderTargetGuard target(renderer_, std::shared_ptr<SDL_Texture>());
    SDL_RenderPresent(renderer_.get());
}
