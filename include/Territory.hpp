#ifndef TERRITORY_HPP
#define TERRITORY_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "HexagonDisplayer.hpp"
#include "Player.hpp"
#include "Cell.hpp"

class Territory: public Cell, public Displayer {
public:
    static const std::string TYPE;

    static void init();
    static const Size getIslandSize();
    static const int getRadius();
    static const int getInnerRadius();

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override;

protected:
    Territory() = default;

    static HexagonDisplayer islandDisplayer;
    static double islandRadius;
    static double islandInnerRadius;
};

#endif
