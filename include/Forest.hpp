#ifndef FOREST_HPP
#define FOREST_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "Player.hpp"
#include "Territory.hpp"

class Forest: public Territory {
public:
    static const std::string TYPE;

    Forest();

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;

    static void init();

protected:
    static Texture* forest_;
};

#endif
