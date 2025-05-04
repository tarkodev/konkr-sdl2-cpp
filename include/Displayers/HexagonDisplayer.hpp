#ifndef HEXAGONDISPLAYER_HPP
#define HEXAGONDISPLAYER_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"  // Provides SDL_Color, basic types and primitives

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for render targets that can be blitted onto
#include "SDLWrappers/Renderers/Texture.hpp"    // RAII wrapper around SDL_Texture

//------------------------------
// Base Displayer
//------------------------------
#include "GenericDisplayer.hpp" // Provides position and size handling

#include <vector>  // std::vector for neighbor flags
#include <memory>  // std::shared_ptr, std::weak_ptr

/**
 * @brief Renders a hexagon tile with optional connection links.
 *
 * GenericDisplayer provides position/size; HexagonDisplayer draws a hexagon
 * texture plus link textures to neighboring hexes based on adjacency flags.
 */
class HexagonDisplayer : public GenericDisplayer {
public:
    /**
     * @brief Constructs a HexagonDisplayer.
     *
     * @param hexagonRadius         Radius (in pixels) of the hexagon tile.
     * @param hexagonTexture        Texture for the main hexagon shape.
     * @param linkTexture           Texture used when two adjacent hexes connect.
     * @param linkBottomLeftTexture Texture for bottom-left connection.
     * @param linkBottomTexture     Texture for bottom connection.
     * @param linkBottomRightTexture Texture for bottom-right connection.
     */
    HexagonDisplayer(double hexagonRadius,
                     const std::shared_ptr<Texture> hexagonTexture,
                     const std::shared_ptr<Texture> linkTexture,
                     const std::shared_ptr<Texture> linkBottomLeftTexture,
                     const std::shared_ptr<Texture> linkBottomTexture,
                     const std::shared_ptr<Texture> linkBottomRightTexture);

    /**
     * @brief Destructor.
     */
    ~HexagonDisplayer();

    /**
     * @brief Draw the hexagon at its stored position without connections.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /**
     * @brief Draw the hexagon at a given position without connections.
     * @param target Weak pointer to the render target.
     * @param pos    Center point in pixel coordinates.
     */
    void display(const std::weak_ptr<BlitTarget>& target, const Point& pos) const override;

    /**
     * @brief Draw the hexagon at a given position with neighbor links.
     * @param target    Weak pointer to the render target.
     * @param pos       Center point in pixel coordinates.
     * @param neighbors Boolean flags for adjacency in each of 6 directions.
     */
    void display(const std::weak_ptr<BlitTarget>& target,
                 const Point& pos,
                 const std::vector<bool>& neighbors) const;

    /**
     * @brief Create a deep copy of this displayer (including cloned textures).
     * @return A new HexagonDisplayer with its own textures.
     */
    HexagonDisplayer copy();

    /**
     * @brief Apply a color modulation to all hexagon and link textures.
     * @param color SDL_Color to tint the textures.
     */
    void colorize(const SDL_Color& color);

private:
    double radius_;        ///< Outer radius of the hexagon in pixels
    double innerRadius_;   ///< Inner radius (width/2) used for positioning links

    std::shared_ptr<Texture> hexagon_;         ///< Main hexagon texture
    std::shared_ptr<Texture> link_;            ///< Generic link texture
    std::shared_ptr<Texture> linkBottomLeft_;  ///< Bottom-left link texture
    std::shared_ptr<Texture> linkBottom_;      ///< Bottom link texture
    std::shared_ptr<Texture> linkBottomRight_; ///< Bottom-right link texture
};

#endif // HEXAGONDISPLAYER_HPP
