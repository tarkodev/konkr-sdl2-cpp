// Button.hpp
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>
#include "Texture.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "Size.hpp"

/**
 * @brief Bouton graphique dont la hitbox est la taille de la texture normale.
 */
class Button {
public:
    using Callback = std::function<void()>;

    /**
     * @param normal    Texture d'affichage normal (définit la taille du bouton)
     * @param hover     Texture au survol (nullptr = pas de changement)
     * @param pressed   Texture quand on presse (nullptr = pas de changement)
     * @param pos       Coin supérieur-gauche du bouton
     */
    Button(Texture* normal,
           Texture* hover,
           Texture* pressed,
           const Point& pos);

    ~Button();

    /// Gère SDL_MOUSEMOTION / SDL_MOUSEBUTTONDOWN / SDL_MOUSEBUTTONUP
    void handleEvent(const SDL_Event& e);

    /// Dessine la bonne texture selon l'état
    void render(SDL_Renderer* renderer) const;

    /// Définit l'action à exécuter au clic
    void setCallback(Callback cb);

private:
    Texture* texNormal_;
    Texture* texHover_;
    Texture* texPressed_;
    Rect     bounds_;

    Callback callback_;
    bool     isHover_;
    bool     isPressed_;
};

#endif // BUTTON_HPP
