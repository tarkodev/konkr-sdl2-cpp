#include "Font.hpp"

Font::Font(const std::string& file, int ptsize) {
    font_ = TTF_OpenFont(file.c_str(), ptsize);
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

TTF_Font* Font::get() const {
    return font_;
}

Texture* Font::renderText(SDL_Renderer* renderer, const std::string& text, SDL_Color color) const {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font_, text.c_str(), color);
    if (!surf) {
        throw std::runtime_error(std::string("TTF_RenderUTF8_Blended failed: ") + TTF_GetError());
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
        throw std::runtime_error(std::string("SDL_CreateTextureFromSurface failed: ") + SDL_GetError());
    }

    // Texture(renderer, SDL_Texture*) interroge automatiquement la taille
    return new Texture(renderer, tex);
}
