#include "FenceDisplayer.hpp"
#include "HexagonUtils.hpp"


FenceDisplayer::FenceDisplayer(double hexagonRadius, 
    Texture* fenceTop, Texture* fenceTopLeft, Texture* fenceTopRight, Texture* fenceBottom, Texture* fenceBottomLeft, Texture* fenceBottomRight,
    Texture* fenceLinkTop, Texture* fenceLinkTopLeft, Texture* fenceLinkTopRight, Texture* fenceLinkBottom, Texture* fenceLinkBottomLeft, Texture* fenceLinkBottomRight
) : radius_(hexagonRadius), innerRadius_(HexagonUtils::radiusToInner(hexagonRadius)),
      top_(fenceTop), topLeft_(fenceTopLeft), topRight_(fenceTopRight), 
      bottom_(fenceBottom), bottomLeft_(fenceBottomLeft), bottomRight_(fenceBottomRight), 
      linkTop_(fenceLinkTop), linkTopLeft_(fenceLinkTopLeft), linkTopRight_(fenceLinkTopRight), 
      linkBottom_(fenceLinkBottom), linkBottomLeft_(fenceLinkBottomLeft), linkBottomRight_(fenceLinkBottomRight)
{}

FenceDisplayer::~FenceDisplayer()
{}


const Size FenceDisplayer::getSize() const {
    return {static_cast<int>(innerRadius_ * 2), static_cast<int>(radius_ * 2)};
}


void FenceDisplayer::display(const BlitTarget* target, const Point& pos, const std::vector<bool>& neighbors) {
    auto [x, y] = pos.get();

    if (!neighbors[5] && !neighbors[0]) {
        target->blit(top_, Point{
            static_cast<int>(x - top_->getWidth() / 2.0),
            static_cast<int>(y - top_->getHeight() / 2.0)
        });
    } else if (!neighbors[5] && neighbors[0]) {
        target->blit(linkTopRight_, Point{
            static_cast<int>(x - 0.5 * innerRadius_ - linkTop_->getWidth() / 2.0),
            static_cast<int>(y - 0.75 * radius_ - linkTop_->getHeight() / 2.0)
        });
    } else if (neighbors[5] && !neighbors[0]) {
        target->blit(linkTopLeft_, Point{
            static_cast<int>(x + 0.5 * innerRadius_ - linkTop_->getWidth() / 2.0),
            static_cast<int>(y - 0.75 * radius_ - linkTop_->getHeight() / 2.0)
        });
    }

    if (!neighbors[0] && !neighbors[1]) {
        target->blit(topLeft_, Point{
            static_cast<int>(x - topLeft_->getWidth() / 2.0),
            static_cast<int>(y - topLeft_->getHeight() / 2.0)
        });
    } else if (!neighbors[0] && neighbors[1]) {
        target->blit(linkTop_, Point{
            static_cast<int>(x - innerRadius_ - linkTop_->getWidth() / 2.0),
            static_cast<int>(y - linkTop_->getHeight() / 2.0)
        });
    }

    if (!neighbors[4] && !neighbors[5]) {
        target->blit(topRight_, Point{
            static_cast<int>(x - topRight_->getWidth() / 2.0),
            static_cast<int>(y - topRight_->getHeight() / 2.0)
        });
    }

    if (!neighbors[1] && !neighbors[2]) {
        target->blit(bottomLeft_, Point{
            static_cast<int>(x - bottomLeft_->getWidth() / 2.0),
            static_cast<int>(y - bottomLeft_->getHeight() / 2.0)
        });
    }

    if (!neighbors[3] && !neighbors[4]) {
        target->blit(bottomRight_, Point{
            static_cast<int>(x - bottomRight_->getWidth() / 2.0),
            static_cast<int>(y - bottomRight_->getHeight() / 2.0)
        });
    } else if (!neighbors[3] && neighbors[4]) {
        target->blit(linkBottom_, Point{
            static_cast<int>(x + innerRadius_ - linkBottom_->getWidth() / 2.0),
            static_cast<int>(y - linkBottom_->getHeight() / 2.0)
        });
    }

    if (!neighbors[2] && !neighbors[3]) {
        target->blit(bottom_, Point{
            static_cast<int>(x - bottom_->getWidth() / 2.0),
            static_cast<int>(y - bottom_->getHeight() / 2.0)
        });
    } else if (neighbors[2] && !neighbors[3]) {
        target->blit(linkBottomRight_, Point{
            static_cast<int>(x - 0.5 * innerRadius_ - linkBottomRight_->getWidth() / 2.0),
            static_cast<int>(y + 0.75 * radius_ - linkBottomRight_->getHeight() / 2.0)
        });
    } else if (!neighbors[2] && neighbors[3]) {
        target->blit(linkBottomLeft_, Point{
            static_cast<int>(x + 0.5 * innerRadius_ - linkBottomLeft_->getWidth() / 2.0),
            static_cast<int>(y + 0.75 * radius_ - linkBottomLeft_->getHeight() / 2.0)
        });
    }
}

void FenceDisplayer::display(const BlitTarget* target, const Point& pos) {
    display(target, pos, {false, false, false, false, false, false});
}

void FenceDisplayer::display(const BlitTarget* target) {
    display(target, pos_, {false, false, false, false, false, false});
}
