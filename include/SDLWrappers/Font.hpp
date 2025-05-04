#ifndef FONT_HPP
#define FONT_HPP

//------------------------------
// Standard Library
//------------------------------
#include <string>       // std::string for file paths and text
#include <memory>       // std::shared_ptr, std::weak_ptr for renderer management

//------------------------------
// SDL2 Core & TTF
//------------------------------
#include "SDL.h"                // SDL_Renderer, SDL_Color
#include "SDL2/SDL_ttf.h"       // TTF_Font and TTF_RenderUTF8_Blended

//------------------------------
// Rendering Abstraction
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"  // Texture wrapper around SDL_Texture

/**
 * @brief Wraps SDL_ttf TTF_Font similar to pygame.font.Font.
 *
 * Usage example:
 *   Font font(renderer, "./assets/fonts/arial.ttf", 24);
 *   Texture textTexture = font.render("Hello, SDL!", {255,255,255,255});
 *   textTexture.blit(...);
 */
class Font {
public:
    /**
     * @brief Constructor: loads a font from file at given size.
     * @param renderer Shared pointer to the SDL_Renderer for creating textures.
     * @param file Path to the TTF font file.
     * @param pointSize Font point size.
     *
     * Throws std::runtime_error on failure.
     */
    Font(const std::shared_ptr<SDL_Renderer>& renderer, const std::string& file, int pointSize);

    /**
     * @brief Destructor: frees the loaded TTF_Font.
     */
    ~Font();

    // Disable copy semantics, allow move semantics
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) noexcept;
    Font& operator=(Font&&) noexcept;

    /**
     * @brief Render a UTF-8 text string into a blended texture.
     * @param text  The UTF-8 text to render.
     * @param color The RGBA color for the text.
     * @return A Texture containing the rendered text.
     *
     * Throws std::runtime_error on failure.
     */
    Texture render(const std::string& text, SDL_Color color);

private:
    /// Weak reference to the SDL_Renderer (shared elsewhere by Window).
    std::weak_ptr<SDL_Renderer> renderer_ = {};

    /// Raw pointer to the TTF_Font instance.
    TTF_Font* font_ = nullptr;
};

#endif // FONT_HPP
