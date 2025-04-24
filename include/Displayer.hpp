#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "Texture.hpp"
#include "Point.hpp"

class Displayer {
public:
    virtual ~Displayer() = default;
    virtual void display(const Texture* target, const Point& pos) = 0;
    virtual const Size& getSize() const = 0;
};

#endif
