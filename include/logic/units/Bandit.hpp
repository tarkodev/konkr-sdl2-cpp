#ifndef UNIT_BANDIT_HPP
#define UNIT_BANDIT_HPP

#include "logic/Troop.hpp"

class Bandit : public Troop {
public:
    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 1;

    explicit Bandit();

    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    static void init();

private:
    static Texture* sprite_;
};

#endif
