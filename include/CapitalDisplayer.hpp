#ifndef CAPITALDISPLAYER_HPP
#define CAPITALDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "Texture.hpp"
#include "BlitTarget.hpp"
#include "GenericDisplayer.hpp"
#include "Font.hpp"

class CapitalDisplayer: public Displayer {
public:
    CapitalDisplayer(Font& font, Point& pos, int capital, int nextCapital);

    ~CapitalDisplayer() = default;

    const Size getSize() const override;
    void display(const BlitTarget* target) override;

private:
    std::unique_ptr<Texture> capital_;
    std::unique_ptr<Texture> bg_;
};

#endif
