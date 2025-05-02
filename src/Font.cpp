#include "Font.hpp"
#include <stdexcept>

Font::Font(const std::shared_ptr<SDL_Renderer>& renderer, const std::string& file, int pointSize)
  : renderer_(renderer)
{
    font_ = TTF_OpenFont(file.c_str(), pointSize);
    if (!font_) {
        throw std::runtime_error(std::string("TTF_OpenFont failed: ") + TTF_GetError());
    }
}

Font::~Font() {
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
}

Font::Font(Font&& o) noexcept
  : renderer_(o.renderer_), font_(o.font_)
{
    o.font_ = nullptr;
}

Font& Font::operator=(Font&& o) noexcept {
    if (this != &o) {
        if (font_) TTF_CloseFont(font_);
        renderer_ = o.renderer_;
        font_     = o.font_;
        o.font_   = nullptr;
    }
    return *this;
}

Texture Font::render(const std::string& text, SDL_Color color) {
    // Render on SDL_Surface
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font_, text.c_str(), color);
    if (!surf)
        throw std::runtime_error(std::string("TTF_RenderUTF8_Blended failed: ") + TTF_GetError());

    // Convert to SDL_Texture
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_.get(), surf);
    SDL_FreeSurface(surf);
    if (!tex)
        throw std::runtime_error(std::string("SDL_CreateTextureFromSurface failed: ") + SDL_GetError());

    // return Texture
    return Texture(renderer_, tex);
}
