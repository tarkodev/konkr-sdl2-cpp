// Forest.cpp
// Implémentation de la logique et de l'affichage pour les cases de type forêt

#include "Cells/Grounds/Forest.hpp"
#include "Cells/Cell.hpp"
#include "Utils/ColorUtils.hpp"

// Stockage statique du sprite de la forêt, partagé entre toutes les instances
std::shared_ptr<Texture> Forest::forest_ = nullptr;


std::shared_ptr<Forest> Forest::cast(const std::weak_ptr<Cell>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Forest>(lobj) : nullptr;
}

const bool Forest::is(const std::weak_ptr<Cell>& obj) {
    return cast(obj) != nullptr;
}

void Forest::init() {
    forest_ = std::make_shared<Texture>(renderer_, "../assets/img/gameelements/forest.png");
}

void Forest::quit() {
    forest_ = nullptr;
}


Forest::Forest(const Point& pos): Ground(pos) {}

void Forest::display(const std::weak_ptr<BlitTarget>& target) const {
    if (auto ltarget = target.lock())
        ltarget->blit(forest_, Point{pos_.getX() - forest_->getWidth() / 2, pos_.getY() - forest_->getHeight() / 2});
}

std::shared_ptr<Cell> Forest::deepCopy() const {
    return std::make_shared<Forest>(pos_);
}
