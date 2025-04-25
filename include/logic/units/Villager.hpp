#ifndef UNIT_VILLAGER_HPP
#define UNIT_VILLAGER_HPP

#include "logic/Troop.hpp"

class Villager : public Troop {
public:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 10;
    static constexpr int UPKEEP   = 2;

    explicit Villager(Player* owner = nullptr);

    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    static void init();

private:
    static Texture* sprite_;
};

#endif
