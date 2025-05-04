#ifndef GENERICDISPLAYER_HPP
#define GENERICDISPLAYER_HPP

#include "SDLWrappers/Coords/Point.hpp"
#include "Displayers/Displayer.hpp"
#include "SDLWrappers/Renderers/BlitTarget.hpp"

class GenericDisplayer: public Displayer {
public:
    GenericDisplayer(const Size& size): Displayer(Point{0, 0}, size) {}

    virtual ~GenericDisplayer() = default;
    
    virtual void display(const std::weak_ptr<BlitTarget>& target, const Point& pos) = 0;
};

#endif
