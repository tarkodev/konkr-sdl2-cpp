#ifndef CAPITALDISPLAYER_HPP
#define CAPITALDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "Texture.hpp"
#include "BlitTarget.hpp"
#include "GenericDisplayer.hpp"
#include "Font.hpp"

class TreasuryDisplayer: public Displayer {
public:
    static void init();
    
    TreasuryDisplayer(const Point& pos, int treasury, int income);

    ~TreasuryDisplayer() = default;

    void setTreasury(int treasury);
    void setIncome(int income);

    const Size getSize() const override;
    void display(const BlitTarget* target) override;

private:
    static std::unique_ptr<Texture> bg_;
    static std::unique_ptr<Font> font_;

    std::unique_ptr<Texture> treasuryTex_;
    int treasury_;
    int income_;
};

#endif
