#ifndef WINDOW_HPP
#define WINDOW_HPP

//------------------------------
// C++ Standard Library
//------------------------------
#include <memory>  // std::unique_ptr, std::shared_ptr

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"   // SDL_Window, SDL_Renderer, SDL_Texture, SDL_Color, SDL_Rect

//------------------------------
// Rendering & Geometry Abstractions
//------------------------------
#include "SDLWrappers/Coords/Size.hpp"         // Encapsulates width/height
#include "SDLWrappers/Renderers/BlitTarget.hpp"// Interface for blit targets
#include "SDLWrappers/Renderers/Texture.hpp"   // Encapsulates SDL_Texture

//------------------------------
// Utilities
//------------------------------
#include "Utils/Checker.hpp"  // Runtime checks and assertions

/**
 * @brief Custom deleter for SDL_Window to ensure proper cleanup.
 */
struct SDLWindowDeleter {
    /**
     * @brief Destroy the SDL_Window if it exists.
     * @param sdlWindow Pointer to the SDL_Window to destroy.
     */
    void operator()(SDL_Window* sdlWindow) const {
        if (sdlWindow) SDL_DestroyWindow(sdlWindow);
    }
};

/**
 * @brief Main application window for the Konkr game.
 *
 * Inherits from BlitTarget to allow textures to be blitted directly
 * onto this window's renderer.
 */
class Window : public BlitTarget {
public:
    /**
     * @brief Create a Window with given title and dimensions.
     * @param title   Window title (UTF-8 C-string).
     * @param size    Initial size (width, height).
     */
    Window(const char* title, const Size& size);

    /**
     * @brief Destructor cleans up renderer and window resources.
     */
    ~Window();

    /**
     * @brief Access the SDL_Renderer for low-level operations.
     * @return Shared pointer to the SDL_Renderer.
     */
    std::shared_ptr<SDL_Renderer> getRenderer() const { return renderer_; }

    /**
     * @brief Get the size of the window.
     * @return Width and height as a Size.
     */
    Size getSize() const override { return size_; }

    /**
     * @brief Get the window width.
     * @return Width in pixels.
     */
    const int getWidth() const { return size_.getWidth(); }

    /**
     * @brief Get the window height.
     * @return Height in pixels.
     */
    const int getHeight() const { return size_.getHeight(); }

    /**
     * @brief Not applicable for windows; always returns nullptr.
     * @return nullptr.
     */
    SDL_Texture* get() const override { return nullptr; }

    /**
     * @brief Fill the entire window with a solid color.
     * @param color SDL_Color to use for filling.
     */
    void fill(const SDL_Color& color) const;

    /**
     * @brief Overlay a semi-transparent black layer to darken the window.
     */
    void darken() const;

    // Overloads for blitting textures onto the window

    void blit(const std::weak_ptr<Texture>& src) const override;
    void blit(const std::weak_ptr<Texture>& src, const Point& destPos) const override;
    void blit(const std::weak_ptr<Texture>& src, const Size& destSize) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& destRect) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Rect& destRect) const override;

    /**
     * @brief Present the current rendering on screen (swap buffers).
     */
    void refresh();

private:
    Size size_ = {0, 0};  ///< Current window size

    ///< SDL_Window wrapped in unique_ptr with custom deleter
    std::unique_ptr<SDL_Window, SDLWindowDeleter> SDLWindow_;

    ///< Renderer associated with the window
    std::shared_ptr<SDL_Renderer> renderer_;

    /**
     * @brief Core blit implementation using raw SDL_Rects.
     * @param src        Source texture (weak ptr).
     * @param srcRect    Source rectangle, or nullptr for full texture.
     * @param destRect   Destination rectangle, or nullptr for full window.
     */
    void blit(const std::weak_ptr<Texture>& src,
              const SDL_Rect* srcRect,
              const SDL_Rect* destRect) const;

    /**
     * @brief Overload for unique_ptr<Texture> sources.
     */
    void blit(const std::unique_ptr<Texture>& src,
              const SDL_Rect* srcRect,
              const SDL_Rect* destRect) const;
};

#endif // WINDOW_HPP
