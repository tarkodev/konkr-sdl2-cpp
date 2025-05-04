#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "SDLWrappers/Coords/Point.hpp"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

class Displayer {
public:
    static void init(const std::shared_ptr<SDL_Renderer>& renderer);
    
    Displayer(const Point& pos = {0, 0}, const Size& size = {0, 0});

    virtual ~Displayer() = default;
    
    virtual void display(const std::weak_ptr<BlitTarget>& target) const = 0;
    
    virtual const Point getPos() const;
    virtual void setPos(const Point& pos);

    virtual Size getSize() const;
    virtual const int getWidth() const;
    virtual const int getHeight() const;


protected:
    static std::weak_ptr<SDL_Renderer> renderer_;

    Size size_;
    Point pos_;
};

#endif
