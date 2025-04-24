#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include "logic/Troop.hpp"

class Villager : public Troop {
public:
    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 1;
    static constexpr int UPKEEP   = 1;

    explicit Villager(Player* owner = nullptr);

    void display(const Texture* target, const Point& pos) override;
};

#endif
