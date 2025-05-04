#ifndef MENUBASE_HPP
#define MENUBASE_HPP

#include "Displayers/Displayer.hpp"
#include "SDLWrappers/Renderers/Window.hpp"
#include "Widgets/Button.hpp"
#include <vector>
#include <memory>

class MenuBase {
public:
    MenuBase(const std::shared_ptr<Window>& window): window_(window) {};

    virtual ~MenuBase() = default;
    
    virtual std::shared_ptr<MenuBase> run() = 0;

protected:
    std::shared_ptr<Window> window_;
    bool loop_ = true;
    std::shared_ptr<MenuBase> nextMenu_;

    virtual void draw() = 0;
    virtual void handleEvents() = 0;
    virtual void handleEvent(const SDL_Event& e);
};

#endif
