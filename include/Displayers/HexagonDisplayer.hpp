#ifndef HEXAGONDISPLAYER_HPP
#define HEXAGONDISPLAYER_HPP

#include "SDL.h"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "GenericDisplayer.hpp"
#include <vector>
#include <memory>

class HexagonDisplayer: public GenericDisplayer {
public:
    HexagonDisplayer(double hexagonRadius,
                     const std::shared_ptr<Texture> hexagonTexture,
                     const std::shared_ptr<Texture> linkTexture,
                     const std::shared_ptr<Texture> linkBottomLeftTexture,
                     const std::shared_ptr<Texture> linkBottomTexture,
                     const std::shared_ptr<Texture> linkBottomRightTexture);

    ~HexagonDisplayer();

    void display(const std::weak_ptr<BlitTarget>& target) const override;
    void display(const std::weak_ptr<BlitTarget>& target, const Point& pos) const override;
    void display(const std::weak_ptr<BlitTarget>& target, const Point& pos, const std::vector<bool>& neighbors) const;

    HexagonDisplayer copy();
    void colorize(const SDL_Color& color);

private:
    double radius_;
    double innerRadius_;
    std::shared_ptr<Texture> hexagon_;
    std::shared_ptr<Texture> link_;
    std::shared_ptr<Texture> linkBottomLeft_;
    std::shared_ptr<Texture> linkBottom_;
    std::shared_ptr<Texture> linkBottomRight_;
};

#endif
