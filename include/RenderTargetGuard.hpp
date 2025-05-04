#ifndef RENDERTARGETGUARD_HPP
#define RENDERTARGETGUARD_HPP

#include "SDL.h"
#include "Utils/Checker.hpp"
#include <memory>
#include <stdexcept>

class RenderTargetGuard {
public:
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture> newTarget): renderer_(renderer) {
        auto lrenderer = renderer_.lock();
        if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");
        
        oldTarget_ = SDL_GetRenderTarget(lrenderer.get());
        SDL_SetRenderTarget(lrenderer.get(), newTarget.get());
    }
    
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<Texture> newTarget): renderer_(renderer) {
        auto lrenderer = renderer_.lock();
        if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");
        
        oldTarget_ = SDL_GetRenderTarget(lrenderer.get());
        SDL_Check(SDL_SetRenderTarget(lrenderer.get(), newTarget->get()), "SDL_SetRenderTarget");
    }

    ~RenderTargetGuard() {
        auto lrenderer = renderer_.lock();
        if (!lrenderer) return;

        SDL_Check(SDL_SetRenderTarget(lrenderer.get(), oldTarget_), "SDL_SetRenderTarget");
    }

private:
    std::weak_ptr<SDL_Renderer> renderer_;
    SDL_Texture* oldTarget_;
};

#endif
