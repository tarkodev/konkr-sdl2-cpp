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

void Texture::recolor(const SDL_Color& hold, const SDL_Color& replace) {
    // 1) Vérifier le format de la texture
    Uint32 format;
    int access, w, h;
    if (SDL_QueryTexture(texture_, &format, &access, &w, &h) != 0)
        throw std::runtime_error("SDL_QueryTexture failed: " + std::string(SDL_GetError()));
    
    if (format != SDL_PIXELFORMAT_RGBA8888)
        throw std::runtime_error("Recolor requires RGBA8888 format");

    // 2) Créer une surface temporaire
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surf)
        throw std::runtime_error("SDL_CreateRGBSurface failed: " + std::string(SDL_GetError()));

    // 3) Lire les pixels depuis la texture
    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer_);
    SDL_SetRenderTarget(renderer_, texture_);
    
    if (SDL_RenderReadPixels(renderer_, 
                           nullptr, 
                           SDL_PIXELFORMAT_RGBA8888,
                           surf->pixels, 
                           surf->pitch) != 0) {
        SDL_FreeSurface(surf);
        SDL_SetRenderTarget(renderer_, oldTarget);
        throw std::runtime_error("SDL_RenderReadPixels failed: " + std::string(SDL_GetError()));
    }
    SDL_SetRenderTarget(renderer_, oldTarget);

    // 4) Modifier les pixels
    auto safeDiv = [](Uint8 v) { return v == 0 ? 0.01f : static_cast<float>(v); };
    
    if (SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);
    
    Uint8* pixels = static_cast<Uint8*>(surf->pixels);
    const int pitch = surf->pitch;
    
    for (int y = 0; y < h; ++y) {
        Uint8* row = pixels + y * pitch;
        for (int x = 0; x < w; ++x) {
            Uint32* pixel = reinterpret_cast<Uint32*>(row + x * 4);
            
            Uint8 r, g, b, a;
            SDL_GetRGBA(*pixel, surf->format, &r, &g, &b, &a);

            const float rf = (r / safeDiv(hold.r)) * replace.r;
            const float gf = (g / safeDiv(hold.g)) * replace.g;
            const float bf = (b / safeDiv(hold.b)) * replace.b;

            const Uint8 rn = std::clamp(rf, 0.0f, 255.0f);
            const Uint8 gn = std::clamp(gf, 0.0f, 255.0f);
            const Uint8 bn = std::clamp(bf, 0.0f, 255.0f);

            *pixel = SDL_MapRGBA(surf->format, rn, gn, bn, a);
        }
    }
    
    if (SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);

    // 5) Créer une NOUVELLE texture avec le format forcé
    SDL_Texture* newTex = SDL_CreateTexture(renderer_, 
                                          SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_STATIC,
                                          w, h);
    if (!newTex) {
        SDL_FreeSurface(surf);
        throw std::runtime_error("SDL_CreateTexture failed: " + std::string(SDL_GetError()));
    }

    // 6) Copier les pixels modifiés dans la nouvelle texture
    if (SDL_UpdateTexture(newTex, nullptr, surf->pixels, surf->pitch) != 0) {
        SDL_DestroyTexture(newTex);
        SDL_FreeSurface(surf);
        throw std::runtime_error("SDL_UpdateTexture failed: " + std::string(SDL_GetError()));
    }

    SDL_FreeSurface(surf);

    // 7) Préserver les propriétés et remplacer l'ancienne texture
    SDL_SetTextureBlendMode(newTex, alpha_ ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
    SDL_DestroyTexture(texture_);
    texture_ = newTex;
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

