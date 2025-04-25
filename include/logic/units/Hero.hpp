#ifndef UNIT_HERO_HPP
#define UNIT_HERO_HPP

#include "logic/Troop.hpp"

class Hero : public Troop {
public:
    static constexpr int STRENGTH = 4;
    static constexpr int COST     = 80;
    static constexpr int UPKEEP   = 54;

    explicit Hero(Player* owner = nullptr);

    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    static void init();

private:
    static Texture* sprite_;
};

#endif
