#include "FenceDisplayer.hpp"
#include "HexagonUtils.hpp"


FenceDisplayer::FenceDisplayer(double hexagonRadius, 
    const std::shared_ptr<Texture>& fenceTop, const std::shared_ptr<Texture>& fenceTopLeft, const std::shared_ptr<Texture>& fenceTopRight, const std::shared_ptr<Texture>& fenceBottom, const std::shared_ptr<Texture>& fenceBottomLeft, const std::shared_ptr<Texture>& fenceBottomRight,
    const std::shared_ptr<Texture>& fenceLinkTop, const std::shared_ptr<Texture>& fenceLinkTopLeft, const std::shared_ptr<Texture>& fenceLinkTopRight, const std::shared_ptr<Texture>& fenceLinkBottom, const std::shared_ptr<Texture>& fenceLinkBottomLeft, const std::shared_ptr<Texture>& fenceLinkBottomRight
) : GenericDisplayer({static_cast<int>(HexagonUtils::radiusToInner(hexagonRadius) * 2), static_cast<int>(hexagonRadius * 2)}), 
      radius_(hexagonRadius), innerRadius_(HexagonUtils::radiusToInner(hexagonRadius)),
      top_(fenceTop), topLeft_(fenceTopLeft), topRight_(fenceTopRight), 
      bottom_(fenceBottom), bottomLeft_(fenceBottomLeft), bottomRight_(fenceBottomRight), 
      linkTop_(fenceLinkTop), linkTopLeft_(fenceLinkTopLeft), linkTopRight_(fenceLinkTopRight), 
      linkBottom_(fenceLinkBottom), linkBottomLeft_(fenceLinkBottomLeft), linkBottomRight_(fenceLinkBottomRight)
{}

FenceDisplayer::~FenceDisplayer()
{}


void FenceDisplayer::display(const std::weak_ptr<BlitTarget>& target, const Point& pos, const std::vector<bool>& neighbors) {
    auto [x, y] = pos.get();
    auto ltarget = target.lock();
    if (!ltarget) return;

    if (!neighbors[5] && !neighbors[0]) {
        ltarget->blit(top_, Point{
            static_cast<int>(x - top_->getWidth() / 2.0),
            static_cast<int>(y - top_->getHeight() / 2.0)
        });
    } else if (!neighbors[5] && neighbors[0]) {
        ltarget->blit(linkTopRight_, Point{
            static_cast<int>(x - 0.5 * innerRadius_ - linkTop_->getWidth() / 2.0),
            static_cast<int>(y - 0.75 * radius_ - linkTop_->getHeight() / 2.0)
        });
    } else if (neighbors[5] && !neighbors[0]) {
        ltarget->blit(linkTopLeft_, Point{
            static_cast<int>(x + 0.5 * innerRadius_ - linkTop_->getWidth() / 2.0),
            static_cast<int>(y - 0.75 * radius_ - linkTop_->getHeight() / 2.0)
        });
    }

    if (!neighbors[0] && !neighbors[1]) {
        ltarget->blit(topLeft_, Point{
            static_cast<int>(x - topLeft_->getWidth() / 2.0),
            static_cast<int>(y - topLeft_->getHeight() / 2.0)
        });
    } else if (!neighbors[0] && neighbors[1]) {
        ltarget->blit(linkTop_, Point{
            static_cast<int>(x - innerRadius_ - linkTop_->getWidth() / 2.0),
            static_cast<int>(y - linkTop_->getHeight() / 2.0)
        });
    }

    if (!neighbors[4] && !neighbors[5]) {
        ltarget->blit(topRight_, Point{
            static_cast<int>(x - topRight_->getWidth() / 2.0),
            static_cast<int>(y - topRight_->getHeight() / 2.0)
        });
    }

    if (!neighbors[1] && !neighbors[2]) {
        ltarget->blit(bottomLeft_, Point{
            static_cast<int>(x - bottomLeft_->getWidth() / 2.0),
            static_cast<int>(y - bottomLeft_->getHeight() / 2.0)
        });
    }

    if (!neighbors[3] && !neighbors[4]) {
        ltarget->blit(bottomRight_, Point{
            static_cast<int>(x - bottomRight_->getWidth() / 2.0),
            static_cast<int>(y - bottomRight_->getHeight() / 2.0)
        });
    } else if (!neighbors[3] && neighbors[4]) {
        ltarget->blit(linkBottom_, Point{
            static_cast<int>(x + innerRadius_ - linkBottom_->getWidth() / 2.0),
            static_cast<int>(y - linkBottom_->getHeight() / 2.0)
        });
    }

    if (!neighbors[2] && !neighbors[3]) {
        ltarget->blit(bottom_, Point{
            static_cast<int>(x - bottom_->getWidth() / 2.0),
            static_cast<int>(y - bottom_->getHeight() / 2.0)
        });
    } else if (neighbors[2] && !neighbors[3]) {
        ltarget->blit(linkBottomRight_, Point{
            static_cast<int>(x - 0.5 * innerRadius_ - linkBottomRight_->getWidth() / 2.0),
            static_cast<int>(y + 0.75 * radius_ - linkBottomRight_->getHeight() / 2.0)
        });
    } else if (!neighbors[2] && neighbors[3]) {
        ltarget->blit(linkBottomLeft_, Point{
            static_cast<int>(x + 0.5 * innerRadius_ - linkBottomLeft_->getWidth() / 2.0),
            static_cast<int>(y + 0.75 * radius_ - linkBottomLeft_->getHeight() / 2.0)
        });
    }
}

void FenceDisplayer::display(const std::weak_ptr<BlitTarget>& target, const Point& pos) {
    display(target, pos, {false, false, false, false, false, false});
}

void FenceDisplayer::display(const std::weak_ptr<BlitTarget>& target) {
    display(target, pos_, {false, false, false, false, false, false});
}
