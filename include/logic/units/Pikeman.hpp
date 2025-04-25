#ifndef UNIT_PIKEMAN_HPP
#define UNIT_PIKEMAN_HPP

#include "logic/Troop.hpp"

class Pikeman : public Troop {
public:
    static constexpr int STRENGTH = 2;
    static constexpr int COST     = 20;
    static constexpr int UPKEEP   = 6;

    explicit Pikeman(Player* owner = nullptr);

    /* Displayer */
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return sprite_->getSize(); }

    /* Chargement des ressources partagées */
    static void init();

private:
    static Texture* sprite_;
};

#endif
