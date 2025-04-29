#pragma once
#include <vector>
#include "Button.hpp"

class MenuBase {
public:
    virtual ~MenuBase() = default;
    virtual void handleEvent(const SDL_Event& e) = 0;
    virtual void render(SDL_Renderer* r) const   = 0;
protected:
    std::vector<Button*> buttons_;
};
