#ifndef FENCEDISPLAYER_HPP
#define FENCEDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "Texture.hpp"
#include "Displayer.hpp"

class FenceDisplayer: public Displayer {
public:
    FenceDisplayer(SDL_Renderer* renderer, double hexagonRadius, 
        Texture* fenceTop, Texture* fenceTopLeft, Texture* fenceTopRight, 
        Texture* fenceBottom, Texture* fenceBottomLeft, Texture* fenceBottomRight,
        Texture* fenceLinkTop, Texture* fenceLinkTopLeft, Texture* fenceLinkTopRight, 
        Texture* fenceLinkBottom, Texture* fenceLinkBottomLeft, Texture* fenceLinkBottomRight
    );

    ~FenceDisplayer();

    const Size getSize() const override;
    void display(const Texture* target, const Point& pos) override;

    void display(const Texture* target, const Point& pos, const std::vector<bool>& neighbors);

    FenceDisplayer copy();

private:
    double radius_;
    double innerRadius_;
    SDL_Renderer* renderer_;
    Texture* top_;
    Texture* topLeft_;
    Texture* topRight_;
    Texture* bottom_;
    Texture* bottomLeft_;
    Texture* bottomRight_;
    Texture* linkTop_;
    Texture* linkTopLeft_;
    Texture* linkTopRight_;
    Texture* linkBottom_;
    Texture* linkBottomLeft_;
    Texture* linkBottomRight_;
};

#endif
