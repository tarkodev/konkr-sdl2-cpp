#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include "SDL.h"
#include "SDL2/SDL_image.h"
#include "Rect.hpp"
#include "Point.hpp"
#include <stdexcept>

/**
 * @brief Classe qui encapsule une SDL_Texture*.
 *
 * Fournit des méthodes pour charger la texture depuis un fichier, la copier sur le renderer,
 * appliquer un "color mod" (colorize) et obtenir ses dimensions.
 */
class Texture {
public:
    /**
     * @brief Construit une texture à partir d'un fichier image.
     * @param renderer Pointeur vers le renderer SDL.
     * @param file Chemin vers le fichier image.
     *
     * Lève une std::runtime_error si le chargement échoue.
     */
    Texture(SDL_Renderer* renderer, const std::string& file);
    Texture(SDL_Renderer *renderer, SDL_Texture* texture);
    Texture(SDL_Renderer *renderer, int w, int h);
    Texture(SDL_Renderer *renderer, const Size& size);

    /**
     * @brief Destructeur.
     *
     * Libère la texture associée.
     */
    ~Texture();

    /**
     * @brief Copie (rend) la texture sur le renderer.
     * 
     * Lève une std::runtime_error en cas d'erreur lors du rendu.
     */
    Texture* copy();

    /**
     * @brief Applique un modulateur de couleur à la texture.
     *
     * La méthode modifie les canaux R, G, et B de la texture sans altérer le canal alpha.
     *
     * @param color La nouvelle couleur à appliquer.
     *
     * Lève une std::runtime_error en cas d'erreur.
     */
    void colorize(const SDL_Color& color);

    /**
     * @brief ecolorize la texture.
     *
     * La méthode modifie les canaux R, G, et B de la texture.
     * @param replace La couleur à remplacer.
     * @param replace La nouvelle couleur à appliquer.
     *
     * Lève une std::runtime_error en cas d'erreur.
     */
    void recolor(const SDL_Color& hold, const SDL_Color& replace);

    /**
     * @brief Retourne les dimensions de la texture sous la forme d'un Point (width, height).
     *
     */
    Size getSize() const;

    /**
     * @brief Retourne la largeur de la texture sous la forme d'un int.
     *
     */
    int getWidth() const;

    /**
     * @brief Retourne la hauteur de la texture sous la forme d'un int.
     *
     */
    int getHeight() const;

    /**
     * @brief Retourne le SDL_Texture* encapsulé.
     */
    SDL_Texture* get() const;

    Texture* convertAlpha();

    Texture* removeAlpha();

    void fill(const SDL_Color& color) const;

    void blit(Texture* src) const;
    void blit(Texture* src, const Point& destPos) const;
    void blit(Texture* src, const Size& destSize) const;
    void blit(Texture* src, const Rect& destRect) const;
    void blit(Texture* src, const Rect& srcRect, const Point& destPos) const;
    void blit(Texture* src, const Rect& srcRect, const Size& destSize) const;
    void blit(Texture* src, const Rect& srcRect, const Rect& destRect) const;

private:
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    Size size_;
    bool alpha_;

    void blit(Texture* src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
};

#endif // TEXTURE_HPP
