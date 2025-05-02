#ifndef TREASURYDISPLAYER_HPP
#define TREASURYDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "Texture.hpp"
#include "BlitTarget.hpp"
#include "GenericDisplayer.hpp"
#include "Font.hpp"

class TreasuryDisplayer: public Displayer {
public:
    static void init();
    
    TreasuryDisplayer(const Point& pos, int treasury = 0, int income = 0);

    ~TreasuryDisplayer() = default;

    void setTreasury(int treasury);
    void setIncome(int income);

    void display(const std::shared_ptr<BlitTarget>& target) override;

private:
    static std::shared_ptr<Texture> bg_;
    static std::shared_ptr<Font> font_;

    std::shared_ptr<Texture> treasuryTex_;
    int treasury_;
    int income_;

    void refreshTexture();
};

#endif
