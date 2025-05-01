#ifndef FONT_HPP
#define FONT_HPP

#include <string>
#include <memory>
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Texture.hpp"

/**
 * @brief Encapsulation de TTF_Font similaire à pygame.font.Font
 *
 * Exemple d'utilisation :
 *   Font font(renderer, "./assets/fonts/arial.ttf", 24);
 *   Texture textTexture = font.render("Hello, SDL!", {255,255,255,255});
 *   textTexture.blit(...);
 */
class Font {
public:
    Font(SDL_Renderer* renderer, const std::string& file, int pointSize);
    ~Font();

    // Interdit copie, autorise move
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) noexcept;
    Font& operator=(Font&&) noexcept;

    /**
     * @brief Rend un texte en créant une texture (blended)
     * @param text La chaîne à afficher (UTF-8)
     * @param color Couleur RGBA du texte
     * @return Une nouvelle Texture contenant le rendu du texte
     */
    Texture render(const std::string& text, SDL_Color color);

private:
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font* font_ = nullptr;
};

#endif // FONT_HPP
