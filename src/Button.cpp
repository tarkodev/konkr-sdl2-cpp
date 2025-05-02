// Button.cpp
#include "Button.hpp"
#include <SDL.h>

Button::Button(const Point& pos, const std::string& normal, const std::string& hover, const std::string& pressed)
    : Displayer(pos)
{
    // Create sprites
    sprite_ = std::make_unique<Texture>(renderer_, normal);
    size_ = sprite_->getSize();

    if (!hover.empty())
        hoverSprite_ = std::make_unique<Texture>(renderer_, hover);

    if (!pressed.empty())
        pressedSprite_ = std::make_unique<Texture>(renderer_, pressed);
}

void Button::setCallback(Callback cb) {
    callback_ = std::move(cb);
}

void Button::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_MOUSEMOTION: {
            Point pos = (Point{e.motion.x, e.motion.y} + size_ / 2);
            isHover_ = Rect{pos_, size_}.contains(pos);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: { 
            Point pos = Point{e.button.x, e.button.y} + size_ / 2;
            isPressed_ = (e.button.button == SDL_BUTTON_LEFT && Rect{pos_, size_}.contains(pos));
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            if (e.button.button == SDL_BUTTON_LEFT) {
                Point pos = Point{e.button.x, e.button.y} + size_ / 2;
                if (callback_ && isPressed_ && Rect{pos_, size_}.contains(pos))
                    callback_();
                isPressed_ = false;
            }
            break;
        }
    }
}

void Button::display(const BlitTarget* target) {
    if (isPressed_ && pressedSprite_)
        target->blit(pressedSprite_.get(), pos_ - (pressedSprite_->getSize() / 2));
    else if (isHover_ && hoverSprite_)
        target->blit(pressedSprite_.get(), pos_ - (hoverSprite_->getSize() / 2));
    else
        target->blit(sprite_.get(), pos_ - (sprite_->getSize() / 2));
}
