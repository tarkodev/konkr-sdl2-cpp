#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"              // Provides SDL_Color, Uint8, etc.

//------------------------------
// Standard Library
//------------------------------
#include <algorithm>          // For std::min
#include <cstdint>            // For fixed-size integer types
#include <string>             // For std::string
#include <stdexcept>          // For std::invalid_argument

/**
 * @namespace ColorUtils
 * @brief Utility functions and constants for working with SDL_Color.
 */
namespace ColorUtils
{
    /**
     * @brief Returns a darker version of the given color.
     *
     * Multiplies the RGB channels by factor f; alpha is unchanged.
     *
     * @param c Base color.
     * @param f Darkening factor (default 0.7).
     * @return SDL_Color Darkened color.
     */
    inline SDL_Color darker(const SDL_Color& c, double f = 0.7) {
        SDL_Color result = c;
        result.r = static_cast<Uint8>(c.r * f);
        result.g = static_cast<Uint8>(c.g * f);
        result.b = static_cast<Uint8>(c.b * f);
        // Alpha channel remains the same
        return result;
    }

    /**
     * @brief Returns a brighter version of the given color.
     *
     * Ensures each channel is at least a minimum value before scaling,
     * then divides by f and clamps to 255.
     *
     * @param c Base color.
     * @param f Brightening factor (default 0.7).
     * @return SDL_Color Brightened color.
     */
    inline SDL_Color brighter(const SDL_Color& c, double f = 0.7) {
        // Compute minimum intensity to avoid too-small values
        double r = c.r, g = c.g, b = c.b;
        double minIntensity = 1.0 / (1.0 - f);
        if (r < minIntensity) r = minIntensity;
        if (g < minIntensity) g = minIntensity;
        if (b < minIntensity) b = minIntensity;

        SDL_Color result;
        // Scale channels and clamp to [0,255]
        result.r = static_cast<Uint8>(std::min(255, static_cast<int>(r / f)));
        result.g = static_cast<Uint8>(std::min(255, static_cast<int>(g / f)));
        result.b = static_cast<Uint8>(std::min(255, static_cast<int>(b / f)));
        result.a = c.a; // Preserve alpha
        return result;
    }

    /**
     * @brief Determines whether black text is appropriate over the given background color.
     *
     * Calculates luminance using 
     *   0.2125*(r/255*100) + 0.7154*(g/255*100) + 0.0721*(b/255*100)
     *
     * @param c Background color.
     * @return true if luminance >= 30, suggesting black text; false otherwise.
     */
    inline const bool textInBlack(const SDL_Color& c) {
        double lum = 0.2125 * (c.r / 255.0 * 100) +
                     0.7154 * (c.g / 255.0 * 100) +
                     0.0721 * (c.b / 255.0 * 100);
        return lum >= 30;
    }

    /**
     * @brief Determines whether white text is appropriate over the given background color.
     *
     * Inverse of textInBlack().
     *
     * @param c Background color.
     * @return true if white text should be used; false otherwise.
     */
    inline const bool textInWhite(const SDL_Color& c) {
        return !textInBlack(c);
    }

    /**
     * @brief Returns a fully transparent version of the given color.
     *
     * Sets the alpha channel to 0; RGB channels unchanged.
     *
     * @param c Original color.
     * @return SDL_Color Transparent color.
     */
    inline SDL_Color toTransparent(const SDL_Color &c) {
        return { c.r, c.g, c.b, 0 };
    }

    //--------------------------------------------------------------------------
    // Predefined Colors
    //--------------------------------------------------------------------------
    // Note: RED, BLUE, and GREEN are currently all set to {255,0,0,255}.
    const SDL_Color RED               = {255,   0,   0, 255};
    const SDL_Color BLUE              = {255,   0,   0, 255};
    const SDL_Color GREEN             = {255,   0,   0, 255};
    const SDL_Color DARK_RED          = {139,   0,   0, 255};
    const SDL_Color YELLOW            = {255, 255,   0, 255};
    const SDL_Color SEABLUE           = { 50, 125, 160, 255};
    const SDL_Color BLACK             = {  0,   0,   0, 255};
    const SDL_Color WHITE             = {255, 255, 255, 255};
    const SDL_Color TRANSPARENT_LOCK  = {  0,   0,   0, 100};
    const SDL_Color TRANSPARENT       = {255, 255, 255,   0};
    const SDL_Color TRANSPARENT_BLACK = {  0,   0,   0,   0};

    /**
     * @brief Parses a HTML-style hex color code into an SDL_Color.
     *
     * Expects strings of the form "#RRGGBB". Throws if format is invalid.
     *
     * @param htmlCode Hex code string, including leading '#'.
     * @return SDL_Color Parsed color with alpha = 255.
     * @throws std::invalid_argument if htmlCode is not "#RRGGBB".
     */
    inline SDL_Color fromHtml(const std::string& htmlCode) {
        if (htmlCode.size() != 7 || htmlCode[0] != '#')
            throw std::invalid_argument("Invalid HTML Code.");

        SDL_Color color;
        color.r = std::stoi(htmlCode.substr(1, 2), nullptr, 16);
        color.g = std::stoi(htmlCode.substr(3, 2), nullptr, 16);
        color.b = std::stoi(htmlCode.substr(5, 2), nullptr, 16);
        color.a = 255;
        return color;
    }

    /**
     * @struct GroundColor
     * @brief Holds a pair of SDL_Colors for ground ownership and availability states.
     */
    struct GroundColor {
        SDL_Color owned;      // Color when ground is owned
        SDL_Color available;  // Color when ground is available
    };

    /**
     * @namespace GroundPalette
     * @brief Predefined palette of ground colors.
     */
    namespace GroundPalette {
        const GroundColor GROUND  = { fromHtml("#886347"), fromHtml("#886347") };
        const GroundColor GREEN   = { fromHtml("#148000"), fromHtml("#106600") };
        const GroundColor ORANGE  = { fromHtml("#f3835d"), fromHtml("#da7553") };
        const GroundColor YELLOW  = { fromHtml("#f2b65d"), fromHtml("#d8a353") };
        const GroundColor BROWN   = { fromHtml("#a27654"), fromHtml("#886347") };
        const GroundColor GREY    = { fromHtml("#6a747c"), fromHtml("#545c62") };
        const GroundColor LIME    = { fromHtml("#7fc83a"), fromHtml("#6fae33") };
        const GroundColor BLUE    = { fromHtml("#3a7fc8"), fromHtml("#336fae") };
        const GroundColor VIOLET  = { fromHtml("#8a3ac8"), fromHtml("#7b33ae") };
        const GroundColor ROSE    = { fromHtml("#c83a7f"), fromHtml("#ae336f") };

        // Array of all ground colors for indexed access
        const GroundColor Palette[] = {
            GROUND, GREEN, ORANGE, YELLOW, BROWN,
            GREY, LIME, BLUE, VIOLET, ROSE
        };
    }

    /**
     * @brief Retrieves a GroundColor by index or returns GREY if out of range.
     *
     * @param index Palette index (0–9).
     * @return GroundColor Palette at index or GREY on invalid index.
     */
    inline GroundColor getGroundColor(const int& index) {
        if (index < 0 || index > 9)
            return GroundPalette::GREY;
        return GroundPalette::Palette[index];
    }
}

#endif // COLORUTILS_HPP
