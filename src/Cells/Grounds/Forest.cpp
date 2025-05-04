#include "Cells/Grounds/Forest.hpp"
#include "Cells/Cell.hpp"
#include "Utils/ColorUtils.hpp"

std::shared_ptr<Texture> Forest::forest_ = nullptr;


std::shared_ptr<Forest> Forest::cast(const std::weak_ptr<Cell>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Forest>(lobj) : nullptr;
}

bool Forest::is(const std::weak_ptr<Cell>& obj) {
    return cast(obj) != nullptr;
}

void Forest::init() {
    forest_ = std::make_shared<Texture>(renderer_, "../assets/img/forest.png");
}

void Forest::quit() {
    forest_ = nullptr;
}


Forest::Forest(const Point& pos): Ground(pos) {}

void Forest::display(const std::weak_ptr<BlitTarget>& target) {
    if (auto ltarget = target.lock())
        ltarget->blit(forest_, Point{pos_.getX() - forest_->getWidth() / 2, pos_.getY() - forest_->getHeight() / 2});
}
