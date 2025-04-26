// Overlay.hpp
#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include <vector>
#include <SDL.h>
#include "Button.hpp"

/**
 * @brief Affiche toujours une collection de boutons par‐dessus la carte.
 */
class Overlay {
public:
    Overlay();
    ~Overlay();

    /// Ajoute un bouton (Overlay prend la propriété et le delete en destructeur)
    void addButton(Button* button);

    /// Doit être appelé depuis votre boucle SDL_PollEvent
    void handleEvent(const SDL_Event& e);

    /// Doit être appelé après avoir dessiné la carte, avant SDL_RenderPresent
    void render(SDL_Renderer* renderer) const;

private:
    std::vector<Button*> buttons_;
};

#endif // OVERLAY_HPP
