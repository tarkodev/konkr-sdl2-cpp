#ifndef TREASURYDISPLAYER_HPP
#define TREASURYDISPLAYER_HPP

#include <vector>
#include "SDL.h"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "SDLWrappers/Renderers/BlitTarget.hpp"
#include "GenericDisplayer.hpp"
#include "SDLWrappers/Font.hpp"

class TreasuryDisplayer: public Displayer {
public:
    static void init();
    static void quit();
    
    TreasuryDisplayer(const Point& pos, int treasury = 0, int income = 0);

    ~TreasuryDisplayer() = default;

    void setTreasury(int treasury);
    void setIncome(int income);
    void setNoIncome(bool noIncome);

    void display(const std::weak_ptr<BlitTarget>& target) const override;

private:
    static std::shared_ptr<Texture> bg_;
    static std::shared_ptr<Font> font_;

    std::shared_ptr<Texture> treasuryTex_;
    int treasury_;
    int income_;
    bool noIncome_ = false;

    void refreshTexture();
};

#endif
