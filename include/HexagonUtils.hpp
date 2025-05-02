#ifndef HEXAGONUTILS_HPP
#define HEXAGONUTILS_HPP

#include <cmath>
#include <utility>
#include <cstdlib>

namespace HexagonUtils
{
    // Constantes
    const double PI = 3.14159265358979323846;
    const double PI2 = 2 * PI;
    const double PI05 = PI / 2.0;

    inline std::pair<int, int> hexRound(double q, double r)
    {
        double s = -q - r;
        int q_round = static_cast<int>(std::round(q));
        int r_round = static_cast<int>(std::round(r));
        int s_round = static_cast<int>(std::round(s));

        double q_diff = std::abs(q_round - q);
        double r_diff = std::abs(r_round - r);
        double s_diff = std::abs(s_round - s);

        if (q_diff > r_diff && q_diff > s_diff)
            q_round = -r_round - s_round;
        else if (r_diff > s_diff)
            r_round = -q_round - s_round;

        return {q_round, r_round};
    }

    /**
     * @brief Convertit une position en pixels vers des coordonnées hexagonales.
     */
    inline std::pair<int, int> pixelToAxial(double x, double y, double hexSize)
    {
        double q = ((std::sqrt(3) / 3.0 * x) - (1.0 / 3.0 * y)) / hexSize;
        double r = (2.0 / 3.0 * y) / hexSize;
        return hexRound(q, r);
    }

    /**
     * @brief Convertit des coordonnées hexagonales en position en pixels.
     */
    inline std::pair<int, int> axialToPixel(double q, double r, double hexSize)
    {
        int x = hexSize * std::sqrt(3) * (q + r / 2.0);
        int y = hexSize * (3.0 / 2.0) * r;
        return {x, y};
    }

    /**
     * @brief Convertit des coordonnées offset (x, y) en coordonnées axiales (q, r).
     */
    inline std::pair<int, int> offsetToAxial(int x, int y)
    {
        int q = x - (y - (y & 1)) / 2;
        int r = y;
        return {q, r};
    }

    /**
     * @brief Convertit des coordonnées axiales (q, r) en coordonnées offset (x, y).
     */
    inline std::pair<int, int> axialToOffset(int q, int r)
    {
        int x = q + ((r - (r & 1)) / 2);
        int y = r;
        return {x, y};
    }

    /**
     * @brief Convertit des coordonnées offset (x, y) en coordonnées axiales (q, r).
     */
    inline std::pair<int, int> offsetToPixel(int x, int y, double hexSize)
    {
        auto [q, r] = offsetToAxial(x, y);
        return axialToPixel(q, r, hexSize);
    }

    /**
     * @brief Convertit des coordonnées axiales (q, r) en coordonnées offset (x, y).
     */
    inline std::pair<int, int> pixelToOffset(int x, int y, double hexSize)
    {
        auto [q, r] = pixelToAxial(x, y, hexSize);
        return axialToOffset(q, r);
    }

    inline double innerToRadius(double radius)
    {
        return radius * 2.0 / std::sqrt(3);
    }

    inline double radiusToInner(double innerRadius)
    {
        return innerRadius * std::sqrt(3) / 2.0;
    }
}

#endif
