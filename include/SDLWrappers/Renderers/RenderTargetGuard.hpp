#ifndef RENDERTARGETGUARD_HPP
#define RENDERTARGETGUARD_HPP

#include "SDL.h"
#include "Utils/Checker.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include <memory>
#include <stdexcept>

class RenderTargetGuard {
public:
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture> newTarget);
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<Texture> newTarget);
    ~RenderTargetGuard();

private:
    std::weak_ptr<SDL_Renderer> renderer_;
    SDL_Texture* oldTarget_;
};

#endif
