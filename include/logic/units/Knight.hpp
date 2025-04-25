#ifndef UNIT_KNIGHT_HPP
#define UNIT_KNIGHT_HPP

#include "logic/Troop.hpp"

class Knight : public Troop {
public:
    static constexpr int STRENGTH = 3;
    static constexpr int COST     = 40;
    static constexpr int UPKEEP   = 18;

    explicit Knight(Player* owner = nullptr);

    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    static void init();

private:
    static Texture* sprite_;
};

#endif
