#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "Texture.hpp"
#include "Point.hpp"

class Displayer {
public:
    Displayer(const Point& pos = {0, 0}): pos_(pos) {}
    virtual ~Displayer() = default;
    virtual void display(const Texture* target) = 0;
    virtual const Size getSize() const = 0;
    virtual void setPos(const Point& pos) { pos_ = pos; }
    virtual Point getPos() const { return pos_; }

protected:
    Point pos_;
};

#endif
