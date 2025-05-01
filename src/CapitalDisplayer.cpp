#include "CapitalDisplayer.hpp"
#include "ColorUtils.hpp"


CapitalDisplayer::CapitalDisplayer(
    Font& font, 
    Point& pos,
    int capital, 
    int nextCapital
) : Displayer(pos)
{
    capital_ = std::make_unique<Texture>(font.render(std::to_string(capital), ColorUtils::BLACK));
    bg_ = std::make_unique<Texture>(renderer_, capital_->getSize());
    bg_->fill(ColorUtils::PURPLE);
}


const Size CapitalDisplayer::getSize() const {
    return bg_->getSize();
}

void CapitalDisplayer::display(const BlitTarget* target) {
    target->blit(bg_.get(), Point{pos_.getX() - bg_->getWidth() / 2, pos_.getY() - bg_->getHeight() / 2});
    target->blit(capital_.get(), Point{pos_.getX() - capital_->getWidth() / 2, pos_.getY() - capital_->getHeight() / 2});
}
