
#include "HexagonUtils.hpp"
#include "HexagonDisplayer.hpp"

HexagonDisplayer::HexagonDisplayer(SDL_Renderer* renderer, double hexagonRadius, Texture* hexagonTexture, Texture* linkTexture, Texture* linkBottomLeftTexture, Texture* linkBottomTexture, Texture* linkBottomRightTexture)
    : renderer_(renderer),
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


const Size HexagonDisplayer::getSize() const {
    return hexagon_->getSize();
}


void HexagonDisplayer::display(const Texture* target, const Point& pos, const std::vector<bool>& neighbors) {
    auto [x, y] = pos.get();
    
    // Draw hexagon
    target->blit(hexagon_, Point{
        static_cast<int>(x - hexagon_->getWidth() / 2.0),
        static_cast<int>(y - hexagon_->getHeight() / 2.0)
    });

    // Draw links
    std::vector<Texture*> links{linkBottomLeft_, linkBottom_, linkBottomRight_};
    for (int i = 0; i < 3; i++) {
        if (!neighbors[i]) continue;

        Texture* link = neighbors[i+1] ? link_ : links[i];
        target->blit(link, Point{
            static_cast<int>(x - (innerRadius_ / (2 - (i%2))) - link->getWidth() / 2.0),
            static_cast<int>(y + ((i-1) * radius_ * 0.75) - link->getHeight() / 2.0)
        });
    }
}

void HexagonDisplayer::display(const Texture* target, const Point& pos) {
    display(target, pos, {false, false, false, false, false, false});
}

void HexagonDisplayer::display(const Texture* target) {
    display(target, pos_, {false, false, false, false, false, false});
}

HexagonDisplayer HexagonDisplayer::copy() {
    return HexagonDisplayer{renderer_, radius_, hexagon_->copy(), link_->copy(), linkBottomLeft_->copy(), linkBottom_->copy(), linkBottomRight_->copy()};
}

void HexagonDisplayer::colorize(const SDL_Color& color) {
    hexagon_->colorize(color);
    link_->colorize(color);
    linkBottomLeft_->colorize(color);
    linkBottom_->colorize(color);
    linkBottomRight_->colorize(color);
}