#include "MenuBase.hpp"

void MenuBase::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT: {
            loop_ = false;
            break;
        }
    }
}