#ifndef UNIT_Castle_HPP
#define UNIT_Castle_HPP

#include "logic/Building.hpp"

class Castle : public Building {
public:
    static constexpr int STRENGTH = 1;
    static constexpr int COST     = 0;
    static constexpr int UPKEEP   = 0;

    explicit Castle(Player* owner = nullptr);

    void onTurnStart() override;   // revenus
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return spriteSize_; }

    static void init(SDL_Renderer* renderer);

private:
    static Texture* sprite_;
    static Size     spriteSize_;
};

#endif
