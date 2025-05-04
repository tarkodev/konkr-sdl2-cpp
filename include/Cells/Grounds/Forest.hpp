#ifndef FOREST_HPP
#define FOREST_HPP

#include "SDL.h"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "GameElements/Player.hpp"
#include "Cells/Grounds/PlayableGround.hpp"

class Forest: public Ground {
public:
    static std::shared_ptr<Forest> cast(const std::weak_ptr<Cell>& obj);
    static bool is(const std::weak_ptr<Cell>& obj);

    static void init();
    static void quit();

    Forest(const Point& pos);

    void display(const std::weak_ptr<BlitTarget>& target) override;

protected:
    static std::shared_ptr<Texture> forest_;
};

#endif
