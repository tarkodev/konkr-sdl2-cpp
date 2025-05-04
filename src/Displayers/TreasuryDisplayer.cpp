#include "Displayers/TreasuryDisplayer.hpp"
#include "Utils/ColorUtils.hpp"

std::shared_ptr<Texture> TreasuryDisplayer::bg_ = nullptr;
std::shared_ptr<Font> TreasuryDisplayer::font_ = nullptr;

void TreasuryDisplayer::init() {
    auto lrenderer = renderer_.lock();
    if (!lrenderer)
        throw std::runtime_error("Displayer not initialized");

    // Load bg
    bg_ = std::make_shared<Texture>(lrenderer, "../assets/img/treasury_bg.png");

    // Load Font
    font_ = std::make_shared<Font>(lrenderer, "../assets/fonts/Inter/static/Inter_18pt-SemiBold.ttf", 25);
}

void TreasuryDisplayer::quit() {
    bg_ = nullptr;
    font_ = nullptr;
}

TreasuryDisplayer::TreasuryDisplayer(
    const Point& pos,
    int treasury, 
    int nextTreasury
) : Displayer(pos, bg_->getSize()), treasury_(treasury), income_(nextTreasury)
{
    if (!bg_)
        throw std::runtime_error("TreasuryDisplayer not initialized");

    refreshTexture();
}

void TreasuryDisplayer::refreshTexture() {
    if (noIncome_)
        treasuryTex_ = std::make_shared<Texture>(font_->render(std::to_string(treasury_), ColorUtils::BLACK));
    else
        treasuryTex_ = std::make_shared<Texture>(font_->render(std::to_string(treasury_) + " + " + std::to_string(income_) + " -> " + std::to_string(treasury_ + income_), income_ < 0 ? ColorUtils::DARK_RED : ColorUtils::BLACK));
}

void TreasuryDisplayer::setTreasury(int treasury) {
    treasury_ = treasury;
    refreshTexture();
}

void TreasuryDisplayer::setIncome(int income) {
    income_ = income;
    refreshTexture();
}

void TreasuryDisplayer::setNoIncome(bool noIncome) {
    noIncome_ = noIncome;
    refreshTexture();
}

void TreasuryDisplayer::display(const std::weak_ptr<BlitTarget>& target) {
    if (auto ltarget = target.lock()) {
        ltarget->blit(bg_, Point{pos_.getX() - bg_->getWidth() / 2, pos_.getY() - bg_->getHeight() / 2});
        ltarget->blit(treasuryTex_, Point{pos_.getX() - treasuryTex_->getWidth() / 2, pos_.getY() - treasuryTex_->getHeight() / 2});
    }
}
