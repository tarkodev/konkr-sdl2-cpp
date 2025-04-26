#ifndef FONT_HPP
#define FONT_HPP

#include <string>
#include <stdexcept>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Texture.hpp"

/**
 * @brief Encapsule TTF_Font* et permet de créer des textures à partir de texte.
 */
class Font {
public:
    /**
     * @param file  Chemin vers le fichier .ttf
     * @param ptsize  Taille en points
     * @throws runtime_error si l'ouverture échoue
     */
    Font(const std::string& file, int ptsize);

    ~Font();

    /// Accès direct au TTF_Font*
    TTF_Font* get() const;

    /**
     * @brief Rend un texte en SDL_Texture via Texture.
     * @param renderer  Renderer SDL
     * @param text      Chaîne UTF-8
     * @param color     Couleur du texte
     * @return pointeur vers une Texture (à delete après usage)
     * @throws runtime_error si rendu ou création échoue
     */
    Texture* renderText(SDL_Renderer* renderer, const std::string& text, SDL_Color color) const;

private:
    TTF_Font* font_;
};

#endif 
