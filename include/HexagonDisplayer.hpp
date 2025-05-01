#ifndef HEXAGONDISPLAYER_HPP
#define HEXAGONDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "BlitTarget.hpp"
#include "Texture.hpp"
#include "GenericDisplayer.hpp"

class HexagonDisplayer: public GenericDisplayer {
public:
    HexagonDisplayer(double hexagonRadius,
                   Texture* hexagonTexture,
                   Texture* linkTexture,
                   Texture* linkBottomLeftTexture,
                   Texture* linkBottomTexture,
                   Texture* linkBottomRightTexture);

    ~HexagonDisplayer();

    const Size getSize() const override;
    void display(const BlitTarget* target) override;
    void display(const BlitTarget* target, const Point& pos) override;

    void display(const BlitTarget* target, const Point& pos, const std::vector<bool>& neighbors);

    HexagonDisplayer copy();
    void colorize(const SDL_Color& color);

private:
    double radius_;
    double innerRadius_;
    Texture* hexagon_;
    Texture* link_;
    Texture* linkBottomLeft_;
    Texture* linkBottom_;
    Texture* linkBottomRight_;
};

#endif
