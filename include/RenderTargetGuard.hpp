#ifndef RENDERTARGETGUARD_HPP
#define RENDERTARGETGUARD_HPP

#include "SDL.h"

class RenderTargetGuard {
public:
    RenderTargetGuard(const std::shared_ptr<SDL_Renderer>& renderer, SDL_Texture* newTarget)
        : renderer_(renderer), oldTarget_(SDL_GetRenderTarget(renderer.get())) {
        SDL_SetRenderTarget(renderer.get(), newTarget);
    }
    
    RenderTargetGuard(const std::shared_ptr<SDL_Renderer>& renderer, Texture* newTarget)
        : renderer_(renderer), oldTarget_(SDL_GetRenderTarget(renderer.get())) {
        SDL_SetRenderTarget(renderer.get(), newTarget->get());
    }

    ~RenderTargetGuard() {
        SDL_SetRenderTarget(renderer_.get(), oldTarget_);
    }

private:
    std::shared_ptr<SDL_Renderer> renderer_;
    SDL_Texture* oldTarget_;
};

#endif
