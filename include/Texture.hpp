#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include "SDL.h"
#include "SDL2/SDL_image.h"
#include "Rect.hpp"
#include "Point.hpp"
#include "BlitTarget.hpp"

#include <stdexcept>
#include <memory>


//! Plus utiliser quand Renderer sera encapsulé dans une classe
struct SDLRendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        if (renderer) SDL_DestroyRenderer(renderer);
    }
};

/**
 * @brief Classe qui encapsule une SDL_Texture*.
 *
 * Fournit des méthodes pour charger la texture depuis un fichier, la copier sur le renderer,
 * appliquer un "color mod" (colorize) et obtenir ses dimensions.
 */
class Texture: public BlitTarget {
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
     * @brief Retourne le SDL_Texture* encapsulé.
     */
    SDL_Texture* get() const override;

    /**
     * @brief Retourne les dimensions de la texture sous la forme d'un Point (width, height).
     *
     */
    Size getSize() const override;

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

    Texture* convertAlpha();

    Texture* removeAlpha();

    void fill(const SDL_Color& color) const;

    void blit(const BlitTarget* src) const override;
    void blit(const BlitTarget* src, const Point& destPos) const override;
    void blit(const BlitTarget* src, const Size& destSize) const override;
    void blit(const BlitTarget* src, const Rect& destRect) const override;
    void blit(const BlitTarget* src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const BlitTarget* src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const BlitTarget* src, const Rect& srcRect, const Rect& destRect) const override;

    void display(const Point& destPos = Point{0, 0});

private:
    SDL_Texture* texture_;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer_; //! encapsuler le renderer dans une classe
    Size size_;
    bool alpha_;

    void blit(const BlitTarget* src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
};

#endif // TEXTURE_HPP
