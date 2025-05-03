#ifndef RENDERTARGETGUARD_HPP
#define RENDERTARGETGUARD_HPP

#include "SDL.h"

class RenderTargetGuard {
public:
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture> newTarget): renderer_(renderer) {
        auto lrenderer = renderer_.lock();
        if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");
        
        oldTarget_ = SDL_GetRenderTarget(lrenderer.get());
        if (SDL_SetRenderTarget(lrenderer.get(), newTarget.get()) != 0)
            throw std::runtime_error("SDL_SetRenderTarget failed: " + std::string(SDL_GetError()));
    }
    
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<Texture> newTarget): renderer_(renderer) {
            auto lrenderer = renderer_.lock();
            if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");
            
            oldTarget_ = SDL_GetRenderTarget(lrenderer.get());
            if (SDL_SetRenderTarget(lrenderer.get(), newTarget->get()) != 0)
                throw std::runtime_error("SDL_SetRenderTarget failed: " + std::string(SDL_GetError()));
        }

    ~RenderTargetGuard() {
        auto lrenderer = renderer_.lock();
        if (!lrenderer) return;
        SDL_SetRenderTarget(lrenderer.get(), oldTarget_);
    }

private:
    std::weak_ptr<SDL_Renderer> renderer_;
    SDL_Texture* oldTarget_;
};

#endif
