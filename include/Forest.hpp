#ifndef FOREST_HPP
#define FOREST_HPP

#include "SDL.h"
#include "Texture.hpp"
#include "BlitTarget.hpp"
#include "Player.hpp"
#include "PlayableGround.hpp"

class Forest: public Ground {
public:
    static const std::string TYPE;

    Forest(const Point& pos);

    const std::string getType() override;
    void display(const BlitTarget* target) override;

    static void init();

protected:
    static Texture* forest_;
};

#endif
