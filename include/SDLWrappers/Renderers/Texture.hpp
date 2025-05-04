#ifndef TEXTURE_HPP
#define TEXTURE_HPP

//------------------------------
// C++ STL
//------------------------------
#include <string>           // std::string
#include <memory>           // std::shared_ptr, std::weak_ptr, std::enable_shared_from_this
#include <stdexcept>        // std::runtime_error

//------------------------------
// SDL2 Core & Extensions
//------------------------------
#include "SDL.h"            // SDL_Renderer, SDL_Texture, SDL_Color, SDL_Rect
#include "SDL2/SDL_image.h" // IMG_Load and image formats support

//------------------------------
// Coordinate Abstractions
//------------------------------
#include "SDLWrappers/Coords/Rect.hpp"   // Encapsulates SDL_Rect
#include "SDLWrappers/Coords/Point.hpp"  // Encapsulates SDL_Point
#include "SDLWrappers/Coords/Size.hpp"   // Encapsulates width/height

//------------------------------
// Rendering Interface
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Defines blit() and display() contracts

/**
 * @brief RAII wrapper around SDL_Texture.
 *
 * Manages creation, rendering, color modulation, and cleanup of an SDL_Texture.
 * Implements BlitTarget for compositing textures and enable_shared_from_this
 * to safely hand out std::shared_ptr<Texture>.
 */
class Texture : public BlitTarget, public std::enable_shared_from_this<Texture> {
public:
    /**
     * @brief Load a texture from an image file.
     * @param renderer Weak pointer to the SDL_Renderer.
     * @param file Path to the image file.
     * @throws std::runtime_error on load failure.
     */
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::string& file);

    /**
     * @brief Wrap an existing SDL_Texture.
     * @param renderer Weak pointer to the SDL_Renderer.
     * @param texture Shared pointer to an already created SDL_Texture.
     */
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture>& texture);

    /**
     * @brief Create an empty texture of given dimensions.
     * @param renderer Weak pointer to the SDL_Renderer.
     * @param w Width in pixels.
     * @param h Height in pixels.
     */
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, int w, int h);

    /**
     * @brief Create an empty texture of given Size.
     * @param renderer Weak pointer to the SDL_Renderer.
     * @param size Size struct (width, height).
     */
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, const Size& size);

    /**
     * @brief Move constructor.
     * Transfers ownership of the underlying SDL_Texture.
     */
    Texture(Texture&& o);

    /**
     * @brief Destructor.
     *
     * Releases the SDL_Texture resource when the last shared_ptr goes out of scope.
     */
    ~Texture();

    // Disable copy operations
    Texture& operator=(const Texture&) = delete;
    // Enable move assignment
    Texture& operator=(Texture&&) noexcept;

    /**
     * @brief Render (copy) this texture to the bound renderer.
     * @return Shared pointer to self for chaining.
     * @throws std::runtime_error on render error.
     */
    std::shared_ptr<Texture> copy();

    /**
     * @brief Apply color modulation to this texture.
     * @param color New RGB values (alpha preserved).
     * @throws std::runtime_error on SDL error.
     */
    void colorize(const SDL_Color& color);

    /**
     * @brief Retrieve the raw SDL_Texture pointer.
     * @return SDL_Texture*
     */
    SDL_Texture* get() const override;

    /**
     * @brief Get texture dimensions.
     * @return Size struct containing width and height.
     */
    Size getSize() const override;

    /**
     * @brief Get texture width.
     * @return Width in pixels.
     */
    const int getWidth() const;

    /**
     * @brief Get texture height.
     * @return Height in pixels.
     */
    const int getHeight() const;

    /**
     * @brief Enable alpha blending on the texture.
     */
    void convertAlpha();

    /**
     * @brief Disable alpha blending (force opaque).
     */
    void removeAlpha();

    /**
     * @brief Fill the entire texture with a solid color.
     * @param color Color to fill (includes alpha).
     */
    void fill(const SDL_Color& color) const;

    // BlitTarget interface: various overloads to copy from another Texture
    void blit(const std::weak_ptr<Texture>& src) const override;
    void blit(const std::weak_ptr<Texture>& src, const Point& destPos) const override;
    void blit(const std::weak_ptr<Texture>& src, const Size& destSize) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& destRect) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Rect& destRect) const override;

    /**
     * @brief Convenience display at a given position.
     * @param destPos Top-left corner where the texture will be rendered.
     */
    void display(const Point& destPos = Point{0, 0}) const;

private:
    std::shared_ptr<SDL_Texture> texture_;   ///< Underlying SDL texture resource
    std::weak_ptr<SDL_Renderer> renderer_;   ///< Renderer used for all draw calls
    Size size_;                              ///< Cached width/height
    bool alpha_;                             ///< True if texture uses alpha blending

    // Internal convenience blit implementations taking raw SDL_Rects
    void blit(const std::weak_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
    void blit(const std::unique_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
};

#endif // TEXTURE_HPP
