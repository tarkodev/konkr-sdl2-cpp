#ifndef RENDERTARGETGUARD_HPP
#define RENDERTARGETGUARD_HPP

#include "SDL.h"

class RenderTargetGuard {
public:
    RenderTargetGuard(SDL_Renderer* renderer, SDL_Texture* newTarget)
        : renderer_(renderer), oldTarget_(SDL_GetRenderTarget(renderer)) {
        SDL_SetRenderTarget(renderer, newTarget);
    }
    
    RenderTargetGuard(SDL_Renderer* renderer, Texture* newTarget)
        : renderer_(renderer), oldTarget_(SDL_GetRenderTarget(renderer)) {
        SDL_SetRenderTarget(renderer, newTarget->get());
    }

    ~RenderTargetGuard() {
        SDL_SetRenderTarget(renderer_, oldTarget_);
    }

private:
    SDL_Renderer* renderer_;
    SDL_Texture* oldTarget_;
};

#endif
