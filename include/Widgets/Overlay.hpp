#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "SDL.h"
#include <vector>
#include <memory>
#include "SDLWrappers/Coords/Point.hpp"
#include "Displayers/Displayer.hpp"
#include "Widgets/Button.hpp"
#include "GameElements/Troops/Troop.hpp"

struct TroopOption {
    std::shared_ptr<Texture>   texNoBuy;
    Point                      texPos;
    std::unique_ptr<Button>    btnBuy;
    bool                       buyable = false;

    Point getPos()  const { return btnBuy->getPos(); }
    Size  getSize() const { return btnBuy->getSize(); }
};

/**
 * @brief Affiche toujours une collection de boutons par‐dessus la carte.
 */
class Overlay: public Displayer {
public:
    Overlay(const Point& pos);

    ~Overlay() = default;

    void handleEvent(const SDL_Event& e);

    bool isHover(const Point& mousePos) const;

    void setPos(const Point& pos) override;
    void display(const std::weak_ptr<BlitTarget>& target) override;

    bool backRequested();
    bool turnRequested();
    bool buyTroopRequested();
    std::shared_ptr<GameElement> getTroopBought();

    void update(int treasury);

private:
    std::shared_ptr<Texture> bg_;

    std::unique_ptr<Button> backBtn_;
    std::vector<TroopOption> options_;
    std::unique_ptr<Button> turnBtn_;

    Point bgPos_;

    bool backRequested_ = false;
    bool turnRequested_ = false;
    char buyTroopRequested_ = '\0';
};

#endif
