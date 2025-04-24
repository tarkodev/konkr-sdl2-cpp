#ifndef TERRITORY_HPP
#define TERRITORY_HPP

#include <SDL.h>
#include "Texture.hpp"
#include "HexagonDisplayer.hpp"
#include "Player.hpp"
#include "Cell.hpp"

class Territory: public Cell, public Displayer {
public:
    static const std::string TYPE;

    static void init();
    static const Size getSpriteSize();
    static const int getRadius();
    static const int getInnerRadius();


    Territory();

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;
    const Size getSize() const override;

protected:
    static HexagonDisplayer plateDisplayer;

private:
    static HexagonDisplayer islandDisplayer_;
    static double islandRadius_;
    static double islandInnerRadius_;
};

#endif
