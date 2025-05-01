#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "Point.hpp"
#include "BlitTarget.hpp"
#include "Texture.hpp"

class Displayer {
public:
    static void init(SDL_Renderer *renderer);
    
    Displayer(const Point& pos = {0, 0});

    virtual ~Displayer() = default;
    
    virtual void display(const BlitTarget* target) = 0; //! display est const ?
    
    virtual const Size getSize() const = 0;
    
    virtual Point getPos() const;
    virtual void setPos(const Point& pos);

protected:
    static SDL_Renderer* renderer_;

    //! Check si toutes classe enfant de Display ont size_ et ajouter size_ ici directement si c'est le cas (et implémenter direct getSize)
    Point pos_;
};

#endif
