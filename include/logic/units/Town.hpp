#ifndef TOWN_HPP
#define TOWN_HPP

#include "logic/Building.hpp"

class Town : public Building {
public:
    static constexpr int UPKEEP = 0;
    static constexpr int STRENGTH = 0;
    static constexpr int COST     = 1;

    explicit Town(Player* owner = nullptr);

    void onTurnStart() override;                  ///< génère l’or du province
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override { return spriteSize_; }

    /* Chargement des ressources partagées */
    static void init(SDL_Renderer* renderer);

private:
    static Texture* sprite_;
    static Size     spriteSize_;
};

#endif
