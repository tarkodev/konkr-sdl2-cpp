#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

#include <SDL.h>
#include <algorithm>
#include <cstdint>

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

const SDL_Color CLR_INDIAN_RED              = {205, 92, 92, 255};
const SDL_Color CLR_LIGHT_CORAL             = {240, 128, 128, 255};
const SDL_Color CLR_SALMON                  = {250, 128, 114, 255};
const SDL_Color CLR_DARK_SALMON             = {233, 150, 122, 255};
const SDL_Color CLR_LIGHT_SALMON            = {255, 160, 122, 255};
const SDL_Color CLR_CRIMSON                 = {220, 20, 60, 255};
const SDL_Color CLR_RED                     = {255, 0, 0, 255};
const SDL_Color CLR_FIRE_BRICK              = {178, 34, 34, 255};
const SDL_Color CLR_DARK_RED                = {139, 0, 0, 255};

const SDL_Color CLR_PINK                    = {255, 192, 203, 255};
const SDL_Color CLR_LIGHT_PINK              = {255, 182, 193, 255};
const SDL_Color CLR_HOT_PINK                = {255, 105, 180, 255};
const SDL_Color CLR_DEEP_PINK               = {255, 20, 147, 255};
const SDL_Color CLR_MEDIUM_VIOLET_RED       = {199, 21, 133, 255};
const SDL_Color CLR_PALE_VIOLET_RED         = {219, 112, 147, 255};

const SDL_Color CLR_CORAL                   = {255, 127, 80, 255};
const SDL_Color CLR_TOMATO                  = {255, 99, 71, 255};
const SDL_Color CLR_ORANGE_RED              = {255, 69, 0, 255};
const SDL_Color CLR_DARK_ORANGE             = {255, 140, 0, 255};
const SDL_Color CLR_ORANGE                  = {255, 165, 0, 255};

const SDL_Color CLR_GOLD                    = {255, 215, 0, 255};
const SDL_Color CLR_YELLOW                  = {255, 255, 0, 255};
const SDL_Color CLR_LIGHT_YELLOW            = {255, 255, 224, 255};
const SDL_Color CLR_LEMON_CHIFFON           = {255, 250, 205, 255};
const SDL_Color CLR_LIGHT_GOLDENROD_YELLOW  = {250, 250, 210, 255};
const SDL_Color CLR_PAPAYA_WHIP             = {255, 239, 213, 255};
const SDL_Color CLR_MOCCASIN                = {255, 228, 181, 255};
const SDL_Color CLR_PEACHPUFF              = {255, 218, 185, 255};
const SDL_Color CLR_PALE_GOLDENROD          = {238, 232, 170, 255};
const SDL_Color CLR_KHAKI                   = {240, 230, 140, 255};
const SDL_Color CLR_DARK_KHAKI              = {189, 183, 107, 255};

const SDL_Color CLR_LAVENDER                = {230, 230, 250, 255};
const SDL_Color CLR_THISTLE                 = {216, 191, 216, 255};
const SDL_Color CLR_PLUM                    = {221, 160, 221, 255};
const SDL_Color CLR_VIOLET                  = {238, 130, 238, 255};
const SDL_Color CLR_ORCHID                  = {218, 112, 214, 255};
const SDL_Color CLR_FUCHSIA                 = {255, 0, 255, 255};
const SDL_Color CLR_MAGENTA                 = {255, 0, 255, 255};
const SDL_Color CLR_MEDIUM_ORCHID           = {186, 85, 211, 255};
const SDL_Color CLR_MEDIUM_PURPLE           = {147, 112, 219, 255};
const SDL_Color CLR_REBECCA_PURPLE          = {102, 51, 153, 255};
const SDL_Color CLR_BLUE_VIOLET             = {138, 43, 226, 255};
const SDL_Color CLR_DARK_VIOLET             = {148, 0, 211, 255};
const SDL_Color CLR_DARK_ORCHID             = {153, 50, 204, 255};
const SDL_Color CLR_DARK_MAGENTA            = {139, 0, 139, 255};
const SDL_Color CLR_PURPLE                  = {128, 0, 128, 255};
const SDL_Color CLR_INDIGO                  = {75, 0, 130, 255};
const SDL_Color CLR_SLATE_BLUE              = {106, 90, 205, 255};
const SDL_Color CLR_DARK_SLATE_BLUE         = {72, 61, 139, 255};
const SDL_Color CLR_MEDIUM_SLATE_BLUE       = {123, 104, 238, 255};

const SDL_Color CLR_GREEN_YELLOW            = {173, 255, 47, 255};
const SDL_Color CLR_CHARTREUSE              = {127, 255, 0, 255};
const SDL_Color CLR_LAWN_GREEN              = {124, 252, 0, 255};
const SDL_Color CLR_LIME                    = {0, 255, 0, 255};
const SDL_Color CLR_LIME_GREEN              = {50, 205, 50, 255};
const SDL_Color CLR_PALE_GREEN              = {152, 251, 152, 255};
const SDL_Color CLR_LIGHT_GREEN             = {144, 238, 144, 255};
const SDL_Color CLR_MEDIUM_SPRING_GREEN     = {0, 250, 154, 255};
const SDL_Color CLR_SPRING_GREEN            = {0, 255, 127, 255};
const SDL_Color CLR_MEDIUM_SEA_GREEN        = {60, 179, 113, 255};
const SDL_Color CLR_SEA_GREEN               = {46, 139, 87, 255};
const SDL_Color CLR_FOREST_GREEN            = {34, 139, 34, 255};
const SDL_Color CLR_GREEN                   = {0, 128, 0, 255};
const SDL_Color CLR_DARK_GREEN              = {0, 100, 0, 255};
const SDL_Color CLR_YELLOW_GREEN            = {154, 205, 50, 255};
const SDL_Color CLR_OLIVE_DRAB              = {107, 142, 35, 255};
const SDL_Color CLR_OLIVE                   = {128, 128, 0, 255};
const SDL_Color CLR_DARK_OLIVE_GREEN        = {85, 107, 47, 255};
const SDL_Color CLR_MEDIUM_AQUAMARINE       = {102, 205, 170, 255};
const SDL_Color CLR_DARK_SEA_GREEN          = {143, 188, 139, 255};
const SDL_Color CLR_LIGHT_SEA_GREEN         = {32, 178, 170, 255};
const SDL_Color CLR_DARK_CYAN               = {0, 139, 139, 255};
const SDL_Color CLR_TEAL                    = {0, 128, 128, 255};

const SDL_Color CLR_AQUA                    = {0, 255, 255, 255};
const SDL_Color CLR_CYAN                    = {0, 255, 255, 255};
const SDL_Color CLR_LIGHT_CYAN              = {224, 255, 255, 255};
const SDL_Color CLR_PALE_TURQUOISE          = {175, 238, 238, 255};
const SDL_Color CLR_AQUAMARINE              = {127, 255, 212, 255};
const SDL_Color CLR_TURQUOISE               = {64, 224, 208, 255};
const SDL_Color CLR_MEDIUM_TURQUOISE        = {72, 209, 204, 255};
const SDL_Color CLR_DARK_TURQUOISE          = {0, 206, 209, 255};
const SDL_Color CLR_CADET_BLUE              = {95, 158, 160, 255};
const SDL_Color CLR_STEEL_BLUE              = {70, 130, 180, 255};
const SDL_Color CLR_LIGHT_STEEL_BLUE        = {176, 196, 222, 255};
const SDL_Color CLR_POWDER_BLUE             = {176, 224, 230, 255};
const SDL_Color CLR_LIGHT_BLUE              = {173, 216, 230, 255};
const SDL_Color CLR_SKY_BLUE                = {135, 206, 235, 255};
const SDL_Color CLR_LIGHT_SKY_BLUE          = {135, 206, 250, 255};
const SDL_Color CLR_DEEP_SKY_BLUE           = {0, 191, 255, 255};
const SDL_Color CLR_DODGER_BLUE             = {30, 144, 255, 255};
const SDL_Color CLR_CORNFLOWER_BLUE         = {100, 149, 237, 255};
const SDL_Color CLR_ROYAL_BLUE              = {65, 105, 225, 255};
const SDL_Color CLR_BLUE                    = {0, 0, 255, 255};
const SDL_Color CLR_MEDIUM_BLUE             = {0, 0, 205, 255};
const SDL_Color CLR_DARK_BLUE               = {0, 0, 139, 255};
const SDL_Color CLR_NAVY                    = {0, 0, 128, 255};
const SDL_Color CLR_MIDNIGHT_BLUE           = {25, 25, 112, 255};

const SDL_Color CLR_CORNSILK                = {255, 248, 220, 255};
const SDL_Color CLR_BLANCHED_ALMOND         = {255, 235, 205, 255};
const SDL_Color CLR_BISQUE                  = {255, 228, 196, 255};
const SDL_Color CLR_NAVAJO_WHITE            = {255, 222, 173, 255};
const SDL_Color CLR_WHEAT                   = {245, 222, 179, 255};
const SDL_Color CLR_BURLY_WOOD              = {222, 184, 135, 255};
const SDL_Color CLR_TAN                     = {210, 180, 140, 255};
const SDL_Color CLR_ROSY_BROWN              = {188, 143, 143, 255};
const SDL_Color CLR_SANDY_BROWN             = {244, 164, 96, 255};
const SDL_Color CLR_GOLDENROD               = {218, 165, 32, 255};
const SDL_Color CLR_DARK_GOLDENROD          = {184, 134, 11, 255};
const SDL_Color CLR_PERU                    = {205, 133, 63, 255};
const SDL_Color CLR_CHOCOLATE               = {210, 105, 30, 255};
const SDL_Color CLR_SADDLE_BROWN            = {139, 69, 19, 255};
const SDL_Color CLR_SIENNA                  = {160, 82, 45, 255};
const SDL_Color CLR_BROWN                   = {165, 42, 42, 255};
const SDL_Color CLR_MAROON                  = {128, 0, 0, 255};

const SDL_Color CLR_WHITE                   = {255, 255, 255, 255};
const SDL_Color CLR_SNOW                    = {255, 250, 250, 255};
const SDL_Color CLR_HONEY_DEW               = {240, 255, 240, 255};
const SDL_Color CLR_MINT_CREAM              = {245, 255, 250, 255};
const SDL_Color CLR_AZURE                   = {240, 255, 255, 255};
const SDL_Color CLR_ALICE_BLUE              = {240, 248, 255, 255};
const SDL_Color CLR_SEABLUE                 = {50, 125, 160, 255};
const SDL_Color CLR_GHOST_WHITE             = {248, 248, 255, 255};
const SDL_Color CLR_WHITE_SMOKE             = {245, 245, 245, 255};
const SDL_Color CLR_SEASHELL                = {255, 245, 238, 255};
const SDL_Color CLR_BEIGE                   = {245, 245, 220, 255};
const SDL_Color CLR_OLD_LACE                = {253, 245, 230, 255};
const SDL_Color CLR_FLORAL_WHITE            = {255, 250, 240, 255};
const SDL_Color CLR_IVORY                   = {255, 255, 240, 255};
const SDL_Color CLR_ANTIQUE_WHITE           = {250, 235, 215, 255};
const SDL_Color CLR_LINEN                   = {250, 240, 230, 255};
const SDL_Color CLR_LAVENDER_BLUSH          = {255, 240, 245, 255};
const SDL_Color CLR_MISTY_ROSE              = {255, 228, 225, 255};

const SDL_Color CLR_GAINSBORO               = {220, 220, 220, 255};
const SDL_Color CLR_LIGHT_GREY              = {211, 211, 211, 255};
const SDL_Color CLR_SILVER                  = {192, 192, 192, 255};
const SDL_Color CLR_DARK_GREY               = {169, 169, 169, 255};
const SDL_Color CLR_GREY                    = {128, 128, 128, 255};
const SDL_Color CLR_DIM_GREY                = {105, 105, 105, 255};
const SDL_Color CLR_LIGHT_SLATE_GREY        = {119, 136, 153, 255};
const SDL_Color CLR_SLATE_GREY              = {112, 128, 144, 255};
const SDL_Color CLR_DARK_SLATE_GREY         = {47, 79, 79, 255};
const SDL_Color CLR_SHADOW_GREY             = {64, 64, 64, 255};
const SDL_Color CLR_BLACK                   = {0, 0, 0, 255};

const SDL_Color CLR_HIGHLIGHT_BLUE          = {50, 100, 250, 255};
const SDL_Color CLR_HIGHLIGHT_GREY          = {80, 80, 80, 255};
const SDL_Color CLR_DEEP_BLUE               = {26, 82, 118, 255};
const SDL_Color CLR_COFFEE                  = {200, 173, 127, 255};
const SDL_Color CLR_TRANSPARENT_LOCK        = {0, 0, 0, 100};
const SDL_Color CLR_TRANSPARENT             = {255, 255, 255, 0};
const SDL_Color CLR_TRANSPARENT_BLACK       = {0, 0, 0, 0};

#endif
