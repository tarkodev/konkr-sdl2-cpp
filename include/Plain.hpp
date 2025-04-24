#ifndef PLAIN_HPP
#define PLAIN_HPP

#include <SDL.h>
#include "Texture.hpp"
#include "Player.hpp"
#include "Territory.hpp"

class Plain: public Territory {
public:
    static const std::string TYPE;

    Plain();

    const std::string getType() override;
    void display(const Texture* target, const Point& pos) override;

    static void init();

protected:
    static HexagonDisplayer displayer_;
};

#endif
