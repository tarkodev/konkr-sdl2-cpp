#ifndef GROUND_HPP
#define GROUND_HPP

#include "SDL.h"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Displayers/Displayer.hpp"
#include "Displayers/HexagonDisplayer.hpp"
#include "Player.hpp"
#include "Cells/Cell.hpp"
#include <memory>

class Ground: public Cell, public Displayer {
public:
    static std::shared_ptr<Ground> cast(const std::weak_ptr<Cell>& obj);
    static bool is(const std::weak_ptr<Cell>& obj);

    static void init();
    static void quit();
    static const Size getIslandSize();
    static const double getRadius();
    static const double getInnerRadius();

    void display(const std::weak_ptr<BlitTarget>& target) override;

protected:
    Ground(const Point& pos);

    static HexagonDisplayer islandDisplayer_;
    static double islandRadius_;
    static double islandInnerRadius_;
};

#endif
