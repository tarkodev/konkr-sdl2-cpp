// Button.hpp
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Displayers/Displayer.hpp"
#include "SDLWrappers/Coords/Rect.hpp"
#include "SDLWrappers/Coords/Point.hpp"
#include "SDLWrappers/Coords/Size.hpp"
#include <functional>
#include <memory>


class Button: public Displayer {
public:
    using Callback = std::function<void()>;

    Button(const Point& pos,
           const std::string& normal,
           const std::string& hover = "",
           const std::string& pressed = "");

    ~Button() = default;

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
  
    Button(Button&&) noexcept = default;
    Button& operator=(Button&&) noexcept = default;

    const bool isHover(const Point& mousePos) const;

    void handleEvent(const SDL_Event& e);
    void setCallback(Callback cb);
    void setPressedCallback(Callback cb);

    void display(const std::weak_ptr<BlitTarget>& target) const override;

private:
    std::shared_ptr<Texture> sprite_;
    std::shared_ptr<Texture> hoverSprite_;
    std::shared_ptr<Texture> pressedSprite_;

    Callback pressedCallback_ = nullptr;
    Callback callback_ = nullptr;
    bool     isHover_ = false;
    bool     isPressed_ = false;
};

#endif
