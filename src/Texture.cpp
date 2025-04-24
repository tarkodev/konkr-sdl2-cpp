#include "Texture.hpp"
#include "DrawUtils.hpp"
#include "ColorUtils.hpp"
#include "Point.hpp"
#include <sstream>

// Constructeur : charge la texture depuis le fichier
Texture::Texture(SDL_Renderer* renderer, const std::string& file): renderer_(renderer)
{
    texture_ = IMG_LoadTexture(renderer, file.c_str());
    if (!texture_)
        throw std::runtime_error("Failed to load texture from " + file + ": " + std::string(SDL_GetError()));

    int w, h;
    if (SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h) != 0)
        throw std::runtime_error("Failed to get size of texture: " + std::string(SDL_GetError()));

    size_ = {w, h};
}

Texture::Texture(SDL_Renderer *renderer, SDL_Texture* texture): renderer_(renderer) {
    texture_ = texture;
    if (!texture_)
        throw std::runtime_error("Texture isn't defined: " + std::string(SDL_GetError()));

    int w, h;
    if (SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h) != 0)
        throw std::runtime_error("Failed to get size of texture: " + std::string(SDL_GetError()));

    size_ = {w, h};

}

Texture::Texture(SDL_Renderer *renderer, int w, int h): renderer_(renderer) {
    texture_ = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!texture_)
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));

    size_ = {w, h};
}

Texture::Texture(SDL_Renderer *renderer, const Size& size): 
    Texture(renderer, size.getWidth(), size.getHeight()) {}


// Destructeur : libère la texture
Texture::~Texture() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Texture::colorize(const SDL_Color& color) {
    SDL_Texture* currentTarget = SDL_GetRenderTarget(renderer_);
    SDL_SetRenderTarget(renderer_, texture_);

    SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
    
    SDL_SetRenderTarget(renderer_, currentTarget);
}

Texture* Texture::copy() {
    Texture* copy = new Texture(renderer_, size_);
    if (alpha_) copy->convertAlpha();

    copy->fill(ColorUtils::TRANSPARENT);
    copy->blit(this);

    return copy;
}

Size Texture::getSize() const {
    return size_;
}

int Texture::getWidth() const {
    return size_.getWidth();
}

int Texture::getHeight() const {
    return size_.getHeight();
}

SDL_Texture* Texture::get() const {
    return texture_;
}

Texture* Texture::convertAlpha() {
    if (!alpha_ && SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND) != 0)
        throw std::runtime_error("Failed to set blend mode of texture: " + std::string(SDL_GetError()));

    alpha_ = true;
    return this;
}

Texture* Texture::removeAlpha() {
    if (alpha_ && SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_NONE) != 0)
        throw std::runtime_error("Failed to reset blend mode of texture: " + std::string(SDL_GetError()));

    alpha_ = false;
    return this;
}

void Texture::fill(const SDL_Color& color) const {
    RenderTargetGuard target(renderer_, texture_);

    DrawUtils::SetRenderDrawColor(renderer_, color);
    if (SDL_RenderClear(renderer_) != 0)
        throw std::runtime_error("Erreur lors du fill de la texture: " + std::string(SDL_GetError()));
}


void Texture::blit(Texture* src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    RenderTargetGuard target(renderer_, texture_);

    if (SDL_RenderCopy(renderer_, src->get(), srcRect, destRect) != 0)
        throw std::runtime_error("Erreur lors du blitting de la texture: " + std::string(SDL_GetError()));
}


void Texture::blit(Texture* src) const {
    blit(src, nullptr, nullptr);
}

void Texture::blit(Texture* src, const Point& destPos) const {
    Rect destRect(destPos, src->getSize());
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(Texture* src, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(Texture* src, const Rect& destRect) const {
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(Texture* src, const Rect& srcRect, const Point& destPos) const {
    Rect destRect(destPos, src->getSize());
    blit(src, &srcRect.get(), &destRect.get());
}

void Texture::blit(Texture* src, const Rect& srcRect, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, &srcRect.get(), &destRect.get());
}

void Texture::blit(Texture* src, const Rect& srcRect, const Rect& destRect) const {
    blit(src, &srcRect.get(), &destRect.get());
}

