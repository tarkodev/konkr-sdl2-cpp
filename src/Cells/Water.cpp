#include "Cells/Water.hpp"

std::shared_ptr<Water> Water::cast(const std::weak_ptr<Cell>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Water>(lobj) : nullptr;
}

const bool Water::is(const std::weak_ptr<Cell>& obj) {
    return cast(obj) != nullptr;
}

std::shared_ptr<Cell> Water::deepCopy() const {
    return std::make_shared<Water>();
}
