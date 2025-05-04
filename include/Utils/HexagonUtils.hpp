#ifndef HEXAGONUTILS_HPP
#define HEXAGONUTILS_HPP

//------------------------------
// Standard Library
//------------------------------
#include <cmath>    // std::round, std::abs, std::sqrt
#include <utility>  // std::pair
#include <cstdlib>  // general utilities (not strictly required here but included)

/**
 * @namespace HexagonUtils
 * @brief Utility functions for hexagonal grid coordinate conversions.
 *
 * Provides conversions between pixel coordinates, axial hex coordinates,
 * offset coordinates, and rounding utilities for fractional hex coordinates.
 */
namespace HexagonUtils
{
    //--------------------------------------------------------------------------
    // Mathematical Constants
    //--------------------------------------------------------------------------
    /// Pi constant
    const double PI   = 3.14159265358979323846;
    /// Two times Pi (full circle)
    const double PI2  = 2 * PI;
    /// Pi over two (90 degrees)
    const double PI05 = PI / 2.0;

    /**
     * @brief Round floating-point axial hex coordinates to the nearest hex.
     *
     * Performs cube-coordinate rounding by comparing the differences
     * in q, r, and s = -q - r, and adjusting the largest error.
     *
     * @param q Fractional axial q coordinate.
     * @param r Fractional axial r coordinate.
     * @return Pair of rounded axial coordinates (q_round, r_round).
     */
    inline std::pair<int, int> hexRound(double q, double r) {
        double s = -q - r;
        int q_round = static_cast<int>(std::round(q));
        int r_round = static_cast<int>(std::round(r));
        int s_round = static_cast<int>(std::round(s));

        double q_diff = std::abs(q_round - q);
        double r_diff = std::abs(r_round - r);
        double s_diff = std::abs(s_round - s);

        // Adjust the component with the largest rounding error
        if (q_diff > r_diff && q_diff > s_diff)
            q_round = -r_round - s_round;
        else if (r_diff > s_diff)
            r_round = -q_round - s_round;

        return {q_round, r_round};
    }

    /**
     * @brief Convert pixel (x, y) to axial hex coordinates.
     *
     * Uses the pointy-top hex layout conversion formulas.
     *
     * @param x       X position in pixels.
     * @param y       Y position in pixels.
     * @param hexSize Radius of the hexagon (distance from center to corner).
     * @return Rounded axial coordinates (q, r).
     */
    inline std::pair<int, int> pixelToAxial(double x, double y, double hexSize) {
        double q = ((std::sqrt(3) / 3.0 * x) - (1.0 / 3.0 * y)) / hexSize;
        double r = (2.0 / 3.0 * y) / hexSize;
        return hexRound(q, r);
    }

    /**
     * @brief Convert axial hex coordinates to pixel (x, y).
     *
     * Uses the pointy-top hex layout conversion formulas.
     *
     * @param q       Axial q coordinate.
     * @param r       Axial r coordinate.
     * @param hexSize Radius of the hexagon.
     * @return Pixel coordinates (x, y).
     */
    inline std::pair<int, int> axialToPixel(double q, double r, double hexSize) {
        int x = static_cast<int>(hexSize * std::sqrt(3) * (q + r / 2.0));
        int y = static_cast<int>(hexSize * (3.0 / 2.0) * r);
        return {x, y};
    }

    /**
     * @brief Convert offset grid coordinates to axial coordinates.
     *
     * Assumes an odd-q vertical layout (columns are offset).
     *
     * @param x Offset column index.
     * @param y Offset row index.
     * @return Axial coordinates (q, r).
     */
    inline std::pair<int, int> offsetToAxial(int x, int y) {
        int q = x - (y - (y & 1)) / 2;
        int r = y;
        return {q, r};
    }

    /**
     * @brief Convert axial hex coordinates to offset grid coordinates.
     *
     * Produces an odd-q vertical layout (columns are offset).
     *
     * @param q Axial q coordinate.
     * @param r Axial r coordinate.
     * @return Offset coordinates (x, y).
     */
    inline std::pair<int, int> axialToOffset(int q, int r) {
        int x = q + ((r - (r & 1)) / 2);
        int y = r;
        return {x, y};
    }

    /**
     * @brief Convert offset grid coordinates directly to pixel coordinates.
     *
     * Chains offsetToAxial and axialToPixel conversions.
     *
     * @param x       Offset column index.
     * @param y       Offset row index.
     * @param hexSize Radius of the hexagon.
     * @return Pixel coordinates (x, y).
     */
    inline std::pair<int, int> offsetToPixel(int x, int y, double hexSize) {
        auto [q, r] = offsetToAxial(x, y);
        return axialToPixel(q, r, hexSize);
    }

    /**
     * @brief Convert pixel coordinates directly to offset grid coordinates.
     *
     * Chains pixelToAxial and axialToOffset conversions.
     *
     * @param x       X position in pixels.
     * @param y       Y position in pixels.
     * @param hexSize Radius of the hexagon.
     * @return Offset coordinates (x, y).
     */
    inline std::pair<int, int> pixelToOffset(int x, int y, double hexSize) {
        auto [q, r] = pixelToAxial(x, y, hexSize);
        return axialToOffset(q, r);
    }

    /**
     * @brief Convert the “inner radius” (apothem) to the outer radius of the hexagon.
     *
     * The inner radius is the distance from the center to the midpoint of any edge.
     *
     * @param radius Outer radius (center to corner).
     * @return Inner radius (center to edge midpoint).
     */
    inline double innerToRadius(double radius) {
        return radius * 2.0 / std::sqrt(3);
    }

    /**
     * @brief Convert the “outer radius” (corner distance) to the inner radius (apothem).
     *
     * @param innerRadius Distance from center to edge midpoint.
     * @return Outer radius (center to corner).
     */
    inline double radiusToInner(double innerRadius) {
        return innerRadius * std::sqrt(3) / 2.0;
    }
}

#endif // HEXAGONUTILS_HPP
