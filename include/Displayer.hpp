#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "Point.hpp"
#include "BlitTarget.hpp"
#include "Texture.hpp"

class Displayer {
public:
    static void init(SDL_Renderer *renderer);
    
    Displayer(const Point& pos = {0, 0}, const Size& size = {0, 0});

    virtual ~Displayer() = default;
    
    virtual void display(const BlitTarget* target) = 0; //! display est const ?
    
    virtual Point getPos() const;
    virtual void setPos(const Point& pos);

    virtual Size getSize() const;
    virtual int getWidth() const;
    virtual int getHeight() const;


protected:
    static SDL_Renderer* renderer_;

    Size size_;
    Point pos_;
};

#endif
