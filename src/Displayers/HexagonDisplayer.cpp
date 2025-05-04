
#include "Utils/HexagonUtils.hpp"
#include "Displayers/HexagonDisplayer.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cmath>

HexagonDisplayer::HexagonDisplayer(double hexagonRadius, const std::shared_ptr<Texture> hexagonTexture, const std::shared_ptr<Texture> linkTexture, const std::shared_ptr<Texture> linkBottomLeftTexture, const std::shared_ptr<Texture> linkBottomTexture, const std::shared_ptr<Texture> linkBottomRightTexture)
    : GenericDisplayer(hexagonTexture ? hexagonTexture->getSize() : Size{0, 0}),
      radius_(hexagonRadius),
      innerRadius_(HexagonUtils::radiusToInner(hexagonRadius)),
      hexagon_(hexagonTexture),
      link_(linkTexture),
      linkBottomLeft_(linkBottomLeftTexture),
      linkBottom_(linkBottomTexture),
      linkBottomRight_(linkBottomRightTexture)
{}

HexagonDisplayer::~HexagonDisplayer()
{}


void HexagonDisplayer::display(const std::weak_ptr<BlitTarget>& target, const Point& pos, const std::vector<bool>& neighbors) {
    auto [x, y] = pos.get();
    auto ltarget = target.lock();
    if (!ltarget) return;
    
    // Draw hexagon
    ltarget->blit(hexagon_, Point{
        static_cast<int>(x - hexagon_->getWidth() / 2.0),
        static_cast<int>(y - hexagon_->getHeight() / 2.0)
    });

    // Draw links
    std::vector<std::shared_ptr<Texture>> links{linkBottomLeft_, linkBottom_, linkBottomRight_};
    for (int i = 0; i < 3; i++) {
        if (!neighbors[i]) continue;

        std::shared_ptr<Texture> link = neighbors[i+1] ? link_ : links[i];
        ltarget->blit(link, Point{
            static_cast<int>(x - (innerRadius_ / (2 - (i%2))) - link->getWidth() / 2.0),
            static_cast<int>(y + ((i-1) * radius_ * 0.75) - link->getHeight() / 2.0)
        });
    }
}

void HexagonDisplayer::display(const std::weak_ptr<BlitTarget>& target, const Point& pos) {
    display(target, pos, {false, false, false, false, false, false});
}

void HexagonDisplayer::display(const std::weak_ptr<BlitTarget>& target) {
    display(target, pos_, {false, false, false, false, false, false});
}

HexagonDisplayer HexagonDisplayer::copy() {
    return HexagonDisplayer{radius_, hexagon_->copy(), link_->copy(), linkBottomLeft_->copy(), linkBottom_->copy(), linkBottomRight_->copy()};
}

void HexagonDisplayer::colorize(const SDL_Color& color) {
    hexagon_->colorize(color);
    link_->colorize(color);
    linkBottomLeft_->colorize(color);
    linkBottom_->colorize(color);
    linkBottomRight_->colorize(color);
}