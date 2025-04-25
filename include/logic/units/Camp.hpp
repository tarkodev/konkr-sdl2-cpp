#ifndef UNIT_CAMP_HPP
#define UNIT_CAMP_HPP

#include "logic/Building.hpp"

class Camp : public Building {
public:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

    explicit Camp();

    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    static void init();

private:
    static Texture* sprite_;
};

#endif
