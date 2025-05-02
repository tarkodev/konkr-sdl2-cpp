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
    Overlay();
    ~Overlay();

    /// Ajoute un bouton (Overlay prend la propriété et le delete en destructeur)
    void addButton(Button* button);

    /// Doit être appelé depuis votre boucle SDL_PollEvent
    void handleEvent(const SDL_Event& e);

    void display(const BlitTarget* target) override;

private:
    std::vector<Button*> buttons_;
};

#endif // OVERLAY_HPP
