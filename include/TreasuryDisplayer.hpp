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
    
    TreasuryDisplayer(const Point& pos, int treasury, int income);

    ~TreasuryDisplayer() = default;

    void setTreasury(int treasury);
    void setIncome(int income);

    void display(const BlitTarget* target) override;

private:
    static std::unique_ptr<Texture> bg_;
    static std::unique_ptr<Font> font_;

    std::unique_ptr<Texture> treasuryTex_;
    int treasury_;
    int income_;

    void refreshTexture();
};

#endif
