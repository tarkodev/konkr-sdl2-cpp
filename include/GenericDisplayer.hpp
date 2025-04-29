#ifndef GENERICDISPLAYER_HPP
#define GENERICDISPLAYER_HPP

#include "Displayer.hpp"
#include "Point.hpp"

class GenericDisplayer: public Displayer {
public:
    GenericDisplayer(): Displayer() {}
    virtual ~GenericDisplayer() = default;
    virtual void display(const Texture* target, const Point& pos) = 0;

protected:
    Point pos_;
};

#endif
