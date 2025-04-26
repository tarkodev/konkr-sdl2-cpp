#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

#include <SDL.h>
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

    const SDL_Color INDIAN_RED              = {205, 92, 92, 255};
    const SDL_Color LIGHT_CORAL             = {240, 128, 128, 255};
    const SDL_Color SALMON                  = {250, 128, 114, 255};
    const SDL_Color DARK_SALMON             = {233, 150, 122, 255};
    const SDL_Color LIGHT_SALMON            = {255, 160, 122, 255};
    const SDL_Color CRIMSON                 = {220, 20, 60, 255};
    const SDL_Color RED                     = {255, 0, 0, 255};
    const SDL_Color FIRE_BRICK              = {178, 34, 34, 255};
    const SDL_Color DARK_RED                = {139, 0, 0, 255};

    const SDL_Color PINK                    = {255, 192, 203, 255};
    const SDL_Color LIGHT_PINK              = {255, 182, 193, 255};
    const SDL_Color HOT_PINK                = {255, 105, 180, 255};
    const SDL_Color DEEP_PINK               = {255, 20, 147, 255};
    const SDL_Color MEDIUM_VIOLET_RED       = {199, 21, 133, 255};
    const SDL_Color PALE_VIOLET_RED         = {219, 112, 147, 255};

    const SDL_Color CORAL                   = {255, 127, 80, 255};
    const SDL_Color TOMATO                  = {255, 99, 71, 255};
    const SDL_Color ORANGE_RED              = {255, 69, 0, 255};
    const SDL_Color DARK_ORANGE             = {255, 140, 0, 255};
    const SDL_Color ORANGE                  = {255, 165, 0, 255};

    const SDL_Color GOLD                    = {255, 215, 0, 255};
    const SDL_Color YELLOW                  = {255, 255, 0, 255};
    const SDL_Color LIGHT_YELLOW            = {255, 255, 224, 255};
    const SDL_Color LEMON_CHIFFON           = {255, 250, 205, 255};
    const SDL_Color LIGHT_GOLDENROD_YELLOW  = {250, 250, 210, 255};
    const SDL_Color PAPAYA_WHIP             = {255, 239, 213, 255};
    const SDL_Color MOCCASIN                = {255, 228, 181, 255};
    const SDL_Color PEACHPUFF              = {255, 218, 185, 255};
    const SDL_Color PALE_GOLDENROD          = {238, 232, 170, 255};
    const SDL_Color KHAKI                   = {240, 230, 140, 255};
    const SDL_Color DARK_KHAKI              = {189, 183, 107, 255};

    const SDL_Color LAVENDER                = {230, 230, 250, 255};
    const SDL_Color THISTLE                 = {216, 191, 216, 255};
    const SDL_Color PLUM                    = {221, 160, 221, 255};
    const SDL_Color VIOLET                  = {238, 130, 238, 255};
    const SDL_Color ORCHID                  = {218, 112, 214, 255};
    const SDL_Color FUCHSIA                 = {255, 0, 255, 255};
    const SDL_Color MAGENTA                 = {255, 0, 255, 255};
    const SDL_Color MEDIUM_ORCHID           = {186, 85, 211, 255};
    const SDL_Color MEDIUM_PURPLE           = {147, 112, 219, 255};
    const SDL_Color REBECCA_PURPLE          = {102, 51, 153, 255};
    const SDL_Color BLUE_VIOLET             = {138, 43, 226, 255};
    const SDL_Color DARK_VIOLET             = {148, 0, 211, 255};
    const SDL_Color DARK_ORCHID             = {153, 50, 204, 255};
    const SDL_Color DARK_MAGENTA            = {139, 0, 139, 255};
    const SDL_Color PURPLE                  = {128, 0, 128, 255};
    const SDL_Color INDIGO                  = {75, 0, 130, 255};
    const SDL_Color SLATE_BLUE              = {106, 90, 205, 255};
    const SDL_Color DARK_SLATE_BLUE         = {72, 61, 139, 255};
    const SDL_Color MEDIUM_SLATE_BLUE       = {123, 104, 238, 255};

    const SDL_Color GREEN_YELLOW            = {173, 255, 47, 255};
    const SDL_Color CHARTREUSE              = {127, 255, 0, 255};
    const SDL_Color LAWN_GREEN              = {124, 252, 0, 255};
    const SDL_Color LIME                    = {0, 255, 0, 255};
    const SDL_Color LIME_GREEN              = {50, 205, 50, 255};
    const SDL_Color PALE_GREEN              = {152, 251, 152, 255};
    const SDL_Color LIGHT_GREEN             = {144, 238, 144, 255};
    const SDL_Color MEDIUM_SPRING_GREEN     = {0, 250, 154, 255};
    const SDL_Color SPRING_GREEN            = {0, 255, 127, 255};
    const SDL_Color MEDIUM_SEA_GREEN        = {60, 179, 113, 255};
    const SDL_Color SEA_GREEN               = {46, 139, 87, 255};
    const SDL_Color FOREST_GREEN            = {34, 139, 34, 255};
    const SDL_Color GREEN                   = {0, 128, 0, 255};
    const SDL_Color DARK_GREEN              = {0, 100, 0, 255};
    const SDL_Color YELLOW_GREEN            = {154, 205, 50, 255};
    const SDL_Color OLIVE_DRAB              = {107, 142, 35, 255};
    const SDL_Color OLIVE                   = {128, 128, 0, 255};
    const SDL_Color DARK_OLIVE_GREEN        = {85, 107, 47, 255};
    const SDL_Color MEDIUM_AQUAMARINE       = {102, 205, 170, 255};
    const SDL_Color DARK_SEA_GREEN          = {143, 188, 139, 255};
    const SDL_Color LIGHT_SEA_GREEN         = {32, 178, 170, 255};
    const SDL_Color DARK_CYAN               = {0, 139, 139, 255};
    const SDL_Color TEAL                    = {0, 128, 128, 255};

    const SDL_Color AQUA                    = {0, 255, 255, 255};
    const SDL_Color CYAN                    = {0, 255, 255, 255};
    const SDL_Color LIGHT_CYAN              = {224, 255, 255, 255};
    const SDL_Color PALE_TURQUOISE          = {175, 238, 238, 255};
    const SDL_Color AQUAMARINE              = {127, 255, 212, 255};
    const SDL_Color TURQUOISE               = {64, 224, 208, 255};
    const SDL_Color MEDIUM_TURQUOISE        = {72, 209, 204, 255};
    const SDL_Color DARK_TURQUOISE          = {0, 206, 209, 255};
    const SDL_Color CADET_BLUE              = {95, 158, 160, 255};
    const SDL_Color STEEL_BLUE              = {70, 130, 180, 255};
    const SDL_Color LIGHT_STEEL_BLUE        = {176, 196, 222, 255};
    const SDL_Color POWDER_BLUE             = {176, 224, 230, 255};
    const SDL_Color LIGHT_BLUE              = {173, 216, 230, 255};
    const SDL_Color SKY_BLUE                = {135, 206, 235, 255};
    const SDL_Color LIGHT_SKY_BLUE          = {135, 206, 250, 255};
    const SDL_Color DEEP_SKY_BLUE           = {0, 191, 255, 255};
    const SDL_Color DODGER_BLUE             = {30, 144, 255, 255};
    const SDL_Color CORNFLOWER_BLUE         = {100, 149, 237, 255};
    const SDL_Color ROYAL_BLUE              = {65, 105, 225, 255};
    const SDL_Color BLUE                    = {0, 0, 255, 255};
    const SDL_Color MEDIUM_BLUE             = {0, 0, 205, 255};
    const SDL_Color DARK_BLUE               = {0, 0, 139, 255};
    const SDL_Color NAVY                    = {0, 0, 128, 255};
    const SDL_Color MIDNIGHT_BLUE           = {25, 25, 112, 255};

    const SDL_Color CORNSILK                = {255, 248, 220, 255};
    const SDL_Color BLANCHED_ALMOND         = {255, 235, 205, 255};
    const SDL_Color BISQUE                  = {255, 228, 196, 255};
    const SDL_Color NAVAJO_WHITE            = {255, 222, 173, 255};
    const SDL_Color WHEAT                   = {245, 222, 179, 255};
    const SDL_Color BURLY_WOOD              = {222, 184, 135, 255};
    const SDL_Color TAN                     = {210, 180, 140, 255};
    const SDL_Color ROSY_BROWN              = {188, 143, 143, 255};
    const SDL_Color SANDY_BROWN             = {244, 164, 96, 255};
    const SDL_Color GOLDENROD               = {218, 165, 32, 255};
    const SDL_Color DARK_GOLDENROD          = {184, 134, 11, 255};
    const SDL_Color PERU                    = {205, 133, 63, 255};
    const SDL_Color CHOCOLATE               = {210, 105, 30, 255};
    const SDL_Color SADDLE_BROWN            = {139, 69, 19, 255};
    const SDL_Color SIENNA                  = {160, 82, 45, 255};
    const SDL_Color BROWN                   = {165, 42, 42, 255};
    const SDL_Color MAROON                  = {128, 0, 0, 255};
    const SDL_Color BROWN2                  = {81, 62, 62, 255};

    const SDL_Color WHITE                   = {255, 255, 255, 255};
    const SDL_Color SNOW                    = {255, 250, 250, 255};
    const SDL_Color HONEY_DEW               = {240, 255, 240, 255};
    const SDL_Color MINT_CREAM              = {245, 255, 250, 255};
    const SDL_Color AZURE                   = {240, 255, 255, 255};
    const SDL_Color ALICE_BLUE              = {240, 248, 255, 255};
    const SDL_Color SEABLUE                 = {50, 125, 160, 255};
    const SDL_Color GHOST_WHITE             = {248, 248, 255, 255};
    const SDL_Color WHITE_SMOKE             = {245, 245, 245, 255};
    const SDL_Color SEASHELL                = {255, 245, 238, 255};
    const SDL_Color BEIGE                   = {245, 245, 220, 255};
    const SDL_Color OLD_LACE                = {253, 245, 230, 255};
    const SDL_Color FLORAL_WHITE            = {255, 250, 240, 255};
    const SDL_Color IVORY                   = {255, 255, 240, 255};
    const SDL_Color ANTIQUE_WHITE           = {250, 235, 215, 255};
    const SDL_Color LINEN                   = {250, 240, 230, 255};
    const SDL_Color LAVENDER_BLUSH          = {255, 240, 245, 255};
    const SDL_Color MISTY_ROSE              = {255, 228, 225, 255};

    const SDL_Color GAINSBORO               = {220, 220, 220, 255};
    const SDL_Color LIGHT_GREY              = {211, 211, 211, 255};
    const SDL_Color SILVER                  = {192, 192, 192, 255};
    const SDL_Color DARK_GREY               = {169, 169, 169, 255};
    const SDL_Color GREY                    = {128, 128, 128, 255};
    const SDL_Color DIM_GREY                = {105, 105, 105, 255};
    const SDL_Color LIGHT_SLATE_GREY        = {119, 136, 153, 255};
    const SDL_Color SLATE_GREY              = {112, 128, 144, 255};
    const SDL_Color DARK_SLATE_GREY         = {47, 79, 79, 255};
    const SDL_Color SHADOW_GREY             = {64, 64, 64, 255};
    const SDL_Color BLACK                   = {0, 0, 0, 255};

    const SDL_Color HIGHLIGHT_BLUE          = {50, 100, 250, 255};
    const SDL_Color HIGHLIGHT_GREY          = {80, 80, 80, 255};
    const SDL_Color DEEP_BLUE               = {26, 82, 118, 255};
    const SDL_Color COFFEE                  = {200, 173, 127, 255};
    const SDL_Color TRANSPARENT_LOCK        = {0, 0, 0, 100};
    const SDL_Color TRANSPARENT             = {255, 255, 255, 0};
    const SDL_Color TRANSPARENT_BLACK       = {0, 0, 0, 0};

    /**
     * @brief Convertit un code HTML hexadécimal en couleur SDL.
     * 
     * Cette fonction prend un code couleur HTML sous la forme "#RRGGBB"
     * et le convertit en structure SDL_Color avec une opacité (alpha) de 255.
     * 
     * @param htmlCode Une chaîne représentant une couleur HTML (ex : "#FF5733").
     * @return SDL_Color La couleur correspondante en format SDL.
     * @throws std::invalid_argument Si le format du code est incorrect.
     */
    inline SDL_Color fromHtml(const std::string& htmlCode) {
        if (htmlCode.size() != 7 || htmlCode[0] != '#')
            throw std::invalid_argument("Code HTML invalide. Format attendu : #RRGGBB");

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

    /**
     * @brief Renvoie la couleur SDL correspondant à un territoire en fonction de son index et de son statut.
     *
     * @param index Un entier de 0 à 6 représentant le type de territoire :
     *        0 = Ground, 1 = Vert, 2 = Orange, 3 = Jaune, 4 = Brun, 5 = Gris, 6 = Lime.
     * @param owned Booléen indiquant si le territoire est possédé (true) ou disponible (false).
     *
     * @return SDL_Color La couleur correspondante.
     */
    inline SDL_Color getGroundColor(int index, bool owned) {
        // Vérifie que l'index est dans les bornes valides
        if (index < 0 || index >= 6) {
            // Retourne Gris comme couleur par défaut si l'index est invalide
            return owned ? GroundPalette::GREY.owned : GroundPalette::GREY.available;
        }

        // Récupère le duo de couleurs (owned / available) selon l'index
        const GroundColor& color = GroundPalette::Palette[index];

        // Retourne la bonne couleur selon la possession du territoire
        return owned ? color.owned : color.available;
    }
}

#endif
