#ifndef FENCEDISPLAYER_HPP
#define FENCEDISPLAYER_HPP

//------------------------------
// STL & Core SDL2
//------------------------------
#include <vector>  // std::vector for neighbor flags
#include "SDL.h"    // SDL_Point, basic SDL types

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"    // Encapsulates SDL_Texture with RAII
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for anything that can be blitted onto

//------------------------------
// Project Interfaces
//------------------------------
#include "GenericDisplayer.hpp" // Base class providing common display utilities

/**
 * @brief Renders hexagonal fence segments based on surrounding neighbors.
 * 
 * FenceDisplayer chooses between straight or “linked” fence textures for each
 * of the six hexagon sides, depending on which neighbors are present.
 */
class FenceDisplayer : public GenericDisplayer {
public:
    /**
     * @brief Construct a FenceDisplayer with all required segment textures.
     * @param hexagonRadius Outer radius of the hexagon cell.
     * @param fenceTop Texture for the top side (no neighbor).
     * @param fenceTopLeft Texture for the top-left side (no neighbor).
     * @param fenceTopRight Texture for the top-right side (no neighbor).
     * @param fenceBottom Texture for the bottom side (no neighbor).
     * @param fenceBottomLeft Texture for the bottom-left side (no neighbor).
     * @param fenceBottomRight Texture for the bottom-right side (no neighbor).
     * @param fenceLinkTop Texture for the top side when linked to a neighbor.
     * @param fenceLinkTopLeft Texture for the top-left side when linked.
     * @param fenceLinkTopRight Texture for the top-right side when linked.
     * @param fenceLinkBottom Texture for the bottom side when linked.
     * @param fenceLinkBottomLeft Texture for the bottom-left side when linked.
     * @param fenceLinkBottomRight Texture for the bottom-right side when linked.
     */
    FenceDisplayer(
        double hexagonRadius,
        const std::shared_ptr<Texture>& fenceTop,
        const std::shared_ptr<Texture>& fenceTopLeft,
        const std::shared_ptr<Texture>& fenceTopRight,
        const std::shared_ptr<Texture>& fenceBottom,
        const std::shared_ptr<Texture>& fenceBottomLeft,
        const std::shared_ptr<Texture>& fenceBottomRight,
        const std::shared_ptr<Texture>& fenceLinkTop,
        const std::shared_ptr<Texture>& fenceLinkTopLeft,
        const std::shared_ptr<Texture>& fenceLinkTopRight,
        const std::shared_ptr<Texture>& fenceLinkBottom,
        const std::shared_ptr<Texture>& fenceLinkBottomLeft,
        const std::shared_ptr<Texture>& fenceLinkBottomRight
    );

    /**
     * @brief Clean up any resources if needed.
     */
    ~FenceDisplayer();

    /**
     * @brief Render fence around this cell’s default position.
     * @param target Weak pointer to the render target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /**
     * @brief Render fence at a specified position.
     * @param target Weak pointer to the render target.
     * @param pos Center point in pixel coordinates.
     */
    void display(const std::weak_ptr<BlitTarget>& target, const Point& pos) const override;

    /**
     * @brief Render fence at a specified position, selecting segments
     *        based on which of the six neighbors exist.
     * @param target Weak pointer to the render target.
     * @param pos Center point in pixel coordinates.
     * @param neighbors Boolean flags for each hex side in this order:
     *        {top, top-right, bottom-right, bottom, bottom-left, top-left}.
     */
    void display(
        const std::weak_ptr<BlitTarget>& target,
        const Point& pos,
        const std::vector<bool>& neighbors
    ) const;

private:
    /// Outer radius of the hexagon cell.
    double radius_;

    /// Inner radius (distance from center to midpoint of a side).
    double innerRadius_;

    /// Textures for fence segments when no neighbor is present.
    std::shared_ptr<Texture> top_;
    std::shared_ptr<Texture> topLeft_;
    std::shared_ptr<Texture> topRight_;
    std::shared_ptr<Texture> bottom_;
    std::shared_ptr<Texture> bottomLeft_;
    std::shared_ptr<Texture> bottomRight_;

    /// Textures for fence segments when linked to a neighbor.
    std::shared_ptr<Texture> linkTop_;
    std::shared_ptr<Texture> linkTopLeft_;
    std::shared_ptr<Texture> linkTopRight_;
    std::shared_ptr<Texture> linkBottom_;
    std::shared_ptr<Texture> linkBottomLeft_;
    std::shared_ptr<Texture> linkBottomRight_;
};

#endif // FENCEDISPLAYER_HPP
