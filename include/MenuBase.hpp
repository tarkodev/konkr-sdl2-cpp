#pragma once
#include <vector>
#include "Displayer.hpp"
#include "Button.hpp"

class MenuBase: public Displayer {
public:
    virtual ~MenuBase() = default;
    virtual void handleEvent(const SDL_Event& e) = 0;

protected:
    std::vector<Button*> buttons_;
};
