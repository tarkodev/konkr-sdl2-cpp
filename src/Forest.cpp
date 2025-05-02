#include "Forest.hpp"
#include "Cell.hpp"
#include "ColorUtils.hpp"

std::shared_ptr<Texture> Forest::forest_ = nullptr;

const std::string Forest::TYPE = "Forest";
const std::string Forest::getType() {
    return Forest::TYPE;
}

Forest::Forest(const Point& pos): Ground(pos) {}

void Forest::init() {
    forest_ = std::make_shared<Texture>(renderer_, "../assets/img/forest.png");
}

void Forest::display(const std::shared_ptr<BlitTarget>& target) {
    target->blit(forest_, Point{pos_.getX() - forest_->getWidth() / 2, pos_.getY() - forest_->getHeight() / 2});
}
