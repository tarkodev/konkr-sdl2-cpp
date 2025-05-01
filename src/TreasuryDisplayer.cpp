#include "TreasuryDisplayer.hpp"
#include "ColorUtils.hpp"

std::unique_ptr<Texture> TreasuryDisplayer::bg_ = nullptr;
std::unique_ptr<Font> TreasuryDisplayer::font_ = nullptr;

void TreasuryDisplayer::init() {
    if (!renderer_)
        throw std::runtime_error("Displayer not initialized");

    // Load bg
    bg_ = std::make_unique<Texture>(renderer_, "../assets/img/treasury_bg.png");
    bg_->convertAlpha();

    // Load Font
    font_ = std::make_unique<Font>(renderer_, "../assets/fonts/Inter/static/Inter_18pt-SemiBold.ttf", 25);
}

TreasuryDisplayer::TreasuryDisplayer(
    const Point& pos,
    int treasury, 
    int nextTreasury
) : Displayer(pos), treasury_(treasury), income_(nextTreasury)
{
    if (!bg_)
        throw std::runtime_error("TreasuryDisplayer not initialized");

    refreshTexture();
}

void TreasuryDisplayer::refreshTexture() {
    treasuryTex_ = std::make_unique<Texture>(font_->render(std::to_string(treasury_) + " + " + std::to_string(income_) + " -> " + std::to_string(treasury_ + income_), income_ < 0 ? ColorUtils::DARK_RED : ColorUtils::BLACK));
}

void TreasuryDisplayer::setTreasury(int treasury) {
    treasury_ = treasury;
    refreshTexture();
}

void TreasuryDisplayer::setIncome(int income) {
    income_ = income;
    refreshTexture();
}


const Size TreasuryDisplayer::getSize() const {
    return bg_->getSize();
}

void TreasuryDisplayer::display(const BlitTarget* target) {
    target->blit(bg_.get(), Point{pos_.getX() - bg_->getWidth() / 2, pos_.getY() - bg_->getHeight() / 2});
    target->blit(treasuryTex_.get(), Point{pos_.getX() - treasuryTex_->getWidth() / 2, pos_.getY() - treasuryTex_->getHeight() / 2});
}
