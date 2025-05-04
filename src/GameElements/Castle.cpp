#include "GameElements/Castle.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"

std::shared_ptr<Texture> Castle::sprite_ = nullptr;

std::shared_ptr<Castle> Castle::cast(const std::weak_ptr<GameElement>& obj) {
    auto lobj = obj.lock();
    return lobj ? std::dynamic_pointer_cast<Castle>(lobj) : nullptr;
}

bool Castle::is(const std::weak_ptr<GameElement>& obj) {
    return cast(obj) != nullptr;
}

void Castle::init()
{
    if (renderer_.expired())
        throw std::runtime_error("Displayer not initialized");
        
    if (sprite_) return;
    sprite_ = std::make_shared<Texture>(renderer_, "../assets/img/builds/castle.png");
}

void Castle::quit()
{
    sprite_ = nullptr;
}


Castle::Castle(const Point& pos): GameElement(pos, sprite_->getSize()) {}

void Castle::display(const std::weak_ptr<BlitTarget>& target) {
    auto ltarget = target.lock();
    if (!ltarget || !sprite_) return;

    ltarget->blit(sprite_, pos_ - sprite_->getSize() / 2);
}
