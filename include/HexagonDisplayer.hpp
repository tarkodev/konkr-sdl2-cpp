#ifndef HEXAGONDISPLAYER_HPP
#define HEXAGONDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "Texture.hpp"
#include "Displayer.hpp"

class HexagonDisplayer: public Displayer {
public:
    HexagonDisplayer(SDL_Renderer* renderer,
                   double hexagonRadius,
                   Texture* hexagonTexture,
                   Texture* linkTexture,
                   Texture* linkBottomLeftTexture,
                   Texture* linkBottomTexture,
                   Texture* linkBottomRightTexture);

    ~HexagonDisplayer();

    const Size getSize() const override;
    void display(const Texture* target, const Point& pos) override;

    void display(const Texture* target, const Point& pos, const std::vector<bool>& neighbors);

    HexagonDisplayer copy();
    void colorize(const SDL_Color& color);

private:
    double radius_;
    double innerRadius_;
    SDL_Renderer* renderer_;
    Texture* hexagon_;
    Texture* link_;
    Texture* linkBottomLeft_;
    Texture* linkBottom_;
    Texture* linkBottomRight_;
};

#endif
