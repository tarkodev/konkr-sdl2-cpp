#ifndef FENCEDISPLAYER_HPP
#define FENCEDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "BlitTarget.hpp"
#include "GenericDisplayer.hpp"

class FenceDisplayer: public GenericDisplayer {
public:
    FenceDisplayer(double hexagonRadius, 
        const std::shared_ptr<Texture>& fenceTop, const std::shared_ptr<Texture>& fenceTopLeft, const std::shared_ptr<Texture>& fenceTopRight, 
        const std::shared_ptr<Texture>& fenceBottom, const std::shared_ptr<Texture>& fenceBottomLeft, const std::shared_ptr<Texture>& fenceBottomRight,
        const std::shared_ptr<Texture>& fenceLinkTop, const std::shared_ptr<Texture>& fenceLinkTopLeft, const std::shared_ptr<Texture>& fenceLinkTopRight, 
        const std::shared_ptr<Texture>& fenceLinkBottom, const std::shared_ptr<Texture>& fenceLinkBottomLeft, const std::shared_ptr<Texture>& fenceLinkBottomRight
    );

    ~FenceDisplayer();

    void display(const std::weak_ptr<BlitTarget>& target) override;
    void display(const std::weak_ptr<BlitTarget>& target, const Point& pos) override;
    void display(const std::weak_ptr<BlitTarget>& target, const Point& pos, const std::vector<bool>& neighbors);

private:
    double radius_;
    double innerRadius_;
    std::shared_ptr<Texture> top_;
    std::shared_ptr<Texture> topLeft_;
    std::shared_ptr<Texture> topRight_;
    std::shared_ptr<Texture> bottom_;
    std::shared_ptr<Texture> bottomLeft_;
    std::shared_ptr<Texture> bottomRight_;
    std::shared_ptr<Texture> linkTop_;
    std::shared_ptr<Texture> linkTopLeft_;
    std::shared_ptr<Texture> linkTopRight_;
    std::shared_ptr<Texture> linkBottom_;
    std::shared_ptr<Texture> linkBottomLeft_;
    std::shared_ptr<Texture> linkBottomRight_;
};

#endif
