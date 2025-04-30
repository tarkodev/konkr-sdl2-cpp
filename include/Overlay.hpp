// Overlay.hpp
#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "SDL.h"
#include <vector>
#include <SDL.h>
#include "Displayer.hpp"
#include "Button.hpp"

/**
 * @brief Affiche toujours une collection de boutons par‐dessus la carte.
 */
class Overlay: public Displayer {
public:
    static void init(SDL_Renderer *renderer);

    Overlay();
    ~Overlay();

    /// Ajoute un bouton (Overlay prend la propriété et le delete en destructeur)
    void addButton(Button* button);

    /// Doit être appelé depuis votre boucle SDL_PollEvent
    void handleEvent(const SDL_Event& e);

    void display(const BlitTarget* target) override;
    const Size getSize() const override { return Size{100, 100}; }; //! Changer quand MenuBase aura une size_

private:
    static SDL_Renderer* renderer_; //! Voir s'il est nécessaire

    std::vector<Button*> buttons_;
};

#endif // OVERLAY_HPP
