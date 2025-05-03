// Overlay.hpp
#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "SDL.h"
#include <vector>
#include <memory>
#include "Point.hpp"
#include "Displayer.hpp"
#include "Button.hpp"

/**
 * @brief Affiche toujours une collection de boutons par‐dessus la carte.
 */
class Overlay: public Displayer {
public:
    Overlay(const Point& pos);

    ~Overlay() = default;

    void handleEvent(const SDL_Event& e);

    void setPos(const Point& pos) override;
    void display(const std::weak_ptr<BlitTarget>& target) override;

    bool backRequested() { return backRequested_; };
    bool nextRequested() { return nextRequested_; };
    bool undoRequested() { return undoRequested_; };
    bool buyTroopRequested() { return buyTroopRequested_; };

private:
    std::shared_ptr<Texture> bg_;
    std::unique_ptr<Button> backBtn_;
    std::unique_ptr<Button> turnBtn_;
    std::vector<Button> troopBtns_;

    Point bgPos_;

    bool backRequested_ = false;
    bool nextRequested_ = false;
    bool undoRequested_ = false;
    bool buyTroopRequested_ = false;
};

#endif
