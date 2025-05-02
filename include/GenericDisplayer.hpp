#ifndef GENERICDISPLAYER_HPP
#define GENERICDISPLAYER_HPP

#include "Point.hpp"
#include "Displayer.hpp"
#include "BlitTarget.hpp"

class GenericDisplayer: public Displayer {
public:
    GenericDisplayer(const Size& size): Displayer(Point{0, 0}, size) {}

    virtual ~GenericDisplayer() = default;
    
    virtual void display(const BlitTarget* target, const Point& pos) = 0;
};

#endif
