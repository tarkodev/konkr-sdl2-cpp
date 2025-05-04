// Button.hpp
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "Displayer.hpp"
#include "Rect.hpp"
#include "Point.hpp"
#include "Size.hpp"
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

    bool isHover(const Point& mousePos) const;

    void handleEvent(const SDL_Event& e);
    void setCallback(Callback cb);
    void setPressedCallback(Callback cb);

    void display(const std::weak_ptr<BlitTarget>& target) override;

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
