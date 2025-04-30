#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "Point.hpp"
#include "BlitTarget.hpp"
#include "Texture.hpp"

class Displayer {
public:
    Displayer(const Point& pos = {0, 0}): pos_(pos) {}
    virtual ~Displayer() = default;
    virtual void display(const BlitTarget* target) = 0; //! display est const ?
    virtual const Size getSize() const = 0;
    virtual void setPos(const Point& pos) { pos_ = pos; }
    virtual Point getPos() const { return pos_; }

protected:
    //! Check si toutes classe enfant de Display on size_ et ajouter si c'est le cas (et implémenter direct getSize)
    Point pos_;
};

#endif
