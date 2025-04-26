#ifndef GROUND_HPP
#define GROUND_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "HexagonDisplayer.hpp"
#include "Player.hpp"
#include "Cell.hpp"

class Ground: public Cell, public Displayer {
public:
    static const std::string TYPE;

    static void init();
    static const Size getIslandSize();
    static const double getRadius();
    static const double getInnerRadius();

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override;

protected:
    Ground() = default;

    static HexagonDisplayer islandDisplayer;
    static double islandRadius;
    static double islandInnerRadius;
};

#endif
