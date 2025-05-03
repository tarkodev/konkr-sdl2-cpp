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

/**
 * @brief Classe qui encapsule une SDL_Texture*.
 *
 * Fournit des méthodes pour charger la texture depuis un fichier, la copier sur le renderer,
 * appliquer un "color mod" (colorize) et obtenir ses dimensions.
 */
class Texture: public BlitTarget, public std::enable_shared_from_this<Texture> {
public:
    /**
     * @brief Construit une texture à partir d'un fichier image.
     * @param renderer Pointeur vers le renderer SDL.
     * @param file Chemin vers le fichier image.
     *
     * Lève une std::runtime_error si le chargement échoue.
     */
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::string& file);
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture>& texture);
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, int w, int h);
    Texture(const std::weak_ptr<SDL_Renderer>& renderer, const Size& size);
    Texture(Texture&& o);

    /**
     * @brief Destructeur.
     *
     * Libère la texture associée.
     */
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) noexcept;


    /**
     * @brief Copie (rend) la texture sur le renderer.
     * 
     * Lève une std::runtime_error en cas d'erreur lors du rendu.
     */
    std::shared_ptr<Texture> copy();

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

    void convertAlpha();

    void removeAlpha();

    void fill(const SDL_Color& color) const;

    void blit(const std::weak_ptr<BlitTarget>& src) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Point& destPos) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Size& destSize) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& destRect) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Point& destPos) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Size& destSize) const override;
    void blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Rect& destRect) const override;

    void display(const Point& destPos = Point{0, 0});

private:
    std::shared_ptr<SDL_Texture> texture_;
    std::weak_ptr<SDL_Renderer> renderer_;
    Size size_;
    bool alpha_;

    void blit(const std::weak_ptr<BlitTarget>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
    void blit(const std::unique_ptr<BlitTarget>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const;
};

#endif
