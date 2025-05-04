#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

#include "SDL.h"
#include <algorithm>
#include <cstdint>
#include <string>       
#include <stdexcept>    

namespace ColorUtils
{
    /**
     * @brief Renvoie une version assombrie de la couleur c.
     *
     * @param c Couleur de base.
     * @param f Facteur d'assombrissement (par défaut 0.7).
     * @return SDL_Color Couleur assombrie (les canaux RGB sont multipliés par f, l'alpha reste inchangé).
     */
    inline SDL_Color darker(const SDL_Color& c, double f = 0.7) {
        SDL_Color result = c;
        result.r = static_cast<Uint8>(c.r * f);
        result.g = static_cast<Uint8>(c.g * f);
        result.b = static_cast<Uint8>(c.b * f);
        return result;
    }

    /**
     * @brief Renvoie une version éclaircie de la couleur c.
     *
     * Pour chaque canal (r, g, b), on s'assure qu'une valeur minimale est présente
     * puis on multiplie par l'inverse de f, en s'assurant de ne pas dépasser 255.
     *
     * @param c Couleur de base.
     * @param f Facteur d'éclaircissement (par défaut 0.7).
     * @return SDL_Color Couleur éclaircie (l'alpha reste inchangé).
     */
    inline SDL_Color brighter(const SDL_Color& c, double f = 0.7) {
        double r = c.r, g = c.g, b = c.b;
        double i = 1.0 / (1.0 - f);
        if (r < i) r = i;
        if (g < i) g = i;
        if (b < i) b = i;
        
        SDL_Color result;
        result.r = static_cast<Uint8>(std::min(255, static_cast<int>(r / f)));
        result.g = static_cast<Uint8>(std::min(255, static_cast<int>(g / f)));
        result.b = static_cast<Uint8>(std::min(255, static_cast<int>(b / f)));
        result.a = c.a;
        return result;
    }

    /**
     * @brief Détermine si le texte doit être en noir selon la luminosité de la couleur c.
     *
     * La luminosité est calculée selon la formule :
     *   lum = 0.2125*(r/255*100) + 0.7154*(g/255*100) + 0.0721*(b/255*100)
     *
     * @param c Couleur de fond.
     * @return true si la luminosité est supérieure ou égale à 30, false sinon.
     */
    inline bool textInBlack(const SDL_Color& c) {
        double lum = 0.2125 * (c.r / 255.0 * 100) +
                    0.7154 * (c.g / 255.0 * 100) +
                    0.0721 * (c.b / 255.0 * 100);
        return lum >= 30;
    }

    /**
     * @brief Détermine si le texte doit être en blanc (l'inverse de textInBlack).
     *
     * @param c Couleur de fond.
     * @return true si le texte doit être en blanc, false sinon.
     */
    inline bool textInWhite(const SDL_Color& c) {
        return !textInBlack(c);
    }

    inline SDL_Color toTransparent(const SDL_Color &c) {
        return {c.r, c.g, c.b, 0};
    }


    /* Define Colors */
    const SDL_Color RED                     = {255, 0, 0, 255};
    const SDL_Color BLUE                    = {255, 0, 0, 255};
    const SDL_Color GREEN                   = {255, 0, 0, 255};
    const SDL_Color DARK_RED                = {139, 0, 0, 255};
    const SDL_Color YELLOW                  = {255, 255, 0, 255};
    const SDL_Color SEABLUE                 = {50, 125, 160, 255};
    const SDL_Color BLACK                   = {0, 0, 0, 255};
    const SDL_Color WHITE                   = {0, 0, 0, 255};
    const SDL_Color TRANSPARENT_LOCK        = {0, 0, 0, 100};
    const SDL_Color TRANSPARENT             = {255, 255, 255, 0};
    const SDL_Color TRANSPARENT_BLACK       = {0, 0, 0, 0};

    
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
    
    struct GroundColor {
        SDL_Color owned;
        SDL_Color available;
    };

    namespace GroundPalette {
        const GroundColor GROUND  = { fromHtml("#886347"), fromHtml("#886347") };
        const GroundColor GREEN   = { fromHtml("#148000"), fromHtml("#106600") };
        const GroundColor ORANGE  = { fromHtml("#f3835d"), fromHtml("#da7553") };
        const GroundColor YELLOW  = { fromHtml("#f2b65d"), fromHtml("#d8a353") };
        const GroundColor BROWN   = { fromHtml("#a27654"), fromHtml("#886347") };
        const GroundColor GREY    = { fromHtml("#6a747c"), fromHtml("#545c62") };
        const GroundColor LIME    = { fromHtml("#7fc83a"), fromHtml("#6fae33") };

        // An array or vector to access by index
        const GroundColor Palette[] = { GROUND, GREEN, ORANGE, YELLOW, BROWN, GREY, LIME };
    }

    inline GroundColor getGroundColor(int index) {
        // Invalid color
        if (index < 0 || index > 6)
            return GroundPalette::GREY;

        // Get good pair of color
        return GroundPalette::Palette[index];
    }
}

#endif
