// Button.cpp
#include "Button.hpp"
#include <SDL.h>

Button::Button(Texture* normal,
               Texture* hover,
               Texture* pressed,
               const Point& pos)
  : texNormal_(normal)
  , texHover_(hover)
  , texPressed_(pressed)
  , callback_(nullptr)
  , isHover_(false)
  , isPressed_(false)
{
    // On crée la hitbox à partir de la taille de la texture normale
    Size sz = normal->getSize();
    bounds_ = Rect{ pos, sz };
}

Button::~Button() {
    // On ne delete pas les textures ici (possiblement partagées)
}

void Button::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_MOUSEMOTION:
            isHover_ = bounds_.contains(e.motion.x, e.motion.y);
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT
                && bounds_.contains(e.button.x, e.button.y)) {
                isPressed_ = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            
            if (e.button.button == SDL_BUTTON_LEFT) {
                if (isPressed_
                    && bounds_.contains(e.button.x, e.button.y)
                    && callback_) {
                    callback_();
                }
                isPressed_ = false;
            }
            break;
    }
}

void Button::render(SDL_Renderer* renderer) const {
    // Choix de la texture selon l'état
    Texture* toDraw = texNormal_;
    if (isPressed_ && texPressed_)   toDraw = texPressed_;
    else if (isHover_ && texHover_)   toDraw = texHover_;

    SDL_Rect dst = bounds_.get();
    SDL_RenderCopy(renderer, toDraw->get(), nullptr, &dst);
}

void Button::setCallback(Callback cb) {
    callback_ = std::move(cb);
}
