#ifndef RENDERTARGETGUARD_HPP
#define RENDERTARGETGUARD_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"                  // Provides SDL_Renderer, SDL_Texture*, SDL_SetRenderTarget, SDL_GetRenderTarget

//------------------------------
// Utilities
//------------------------------
#include "Utils/Checker.hpp"      // Inline function SDL_Check to validate SDL calls

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp" // Wrapper for SDL_Texture with RAII

#include <memory>      // std::weak_ptr, std::shared_ptr
#include <stdexcept>   // std::runtime_error

/**
 * @brief RAII guard to temporarily change the SDL render target.
 *
 * On construction, saves the current render target of the SDL_Renderer,
 * then sets the provided new target (SDL_Texture* or Texture wrapper).
 * On destruction, restores the original render target automatically.
 */
class RenderTargetGuard {
public:
    /**
     * @brief Create a guard using a raw SDL_Texture.
     * @param renderer Weak pointer to the SDL_Renderer owning the target.
     * @param newTarget Shared pointer to the new SDL_Texture target.
     *
     * Throws std::runtime_error if the renderer is not initialized.
     */
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer,
                      const std::shared_ptr<SDL_Texture> newTarget);

    /**
     * @brief Create a guard using our Texture wrapper.
     * @param renderer Weak pointer to the SDL_Renderer owning the target.
     * @param newTarget Shared pointer to our Texture (wrapping an SDL_Texture).
     *
     * Throws std::runtime_error if the renderer is not initialized.
     */
    RenderTargetGuard(const std::weak_ptr<SDL_Renderer>& renderer,
                      const std::shared_ptr<Texture> newTarget);

    /**
     * @brief Destructor restores the original render target.
     * Automatically called when the guard goes out of scope.
     */
    ~RenderTargetGuard();

private:
    /// Weak reference to the SDL_Renderer to call SDL_SetRenderTarget.
    std::weak_ptr<SDL_Renderer> renderer_;

    /// Raw pointer to the old SDL_Texture target, saved at construction.
    SDL_Texture* oldTarget_;
};

#endif // RENDERTARGETGUARD_HPP
