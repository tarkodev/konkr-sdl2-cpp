#include "Texture.hpp"
#include "ColorUtils.hpp"
#include "Point.hpp"
#include "RenderTargetGuard.hpp"
#include <sstream>

// Constructeur : charge la texture depuis le fichier
Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::string& file): renderer_(renderer) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");

    SDL_Texture* texture = IMG_LoadTexture(lrenderer.get(), file.c_str());
    if (!texture)
        throw std::runtime_error("Failed to load texture from " + file + ": " + std::string(SDL_GetError()));

    int w, h;
    if (SDL_QueryTexture(texture, nullptr, nullptr, &w, &h) != 0)
        throw std::runtime_error("Failed to get size of texture: " + std::string(SDL_GetError()));

    texture_ = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);

    convertAlpha();
    size_ = {w, h};
}

Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture>& texture): renderer_(renderer) {
    texture_ = texture;
    if (!texture_)
        throw std::runtime_error("Texture isn't defined: " + std::string(SDL_GetError()));

    int w, h;
    if (SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h) != 0)
        throw std::runtime_error("Failed to get size of texture: " + std::string(SDL_GetError()));

    convertAlpha();
    size_ = {w, h};
}

Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, int w, int h): renderer_(renderer) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");

    texture_ = std::shared_ptr<SDL_Texture>(SDL_CreateTexture(lrenderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h), SDL_DestroyTexture);
    if (!texture_)
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));

    convertAlpha();
    size_ = {w, h};
}

Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, const Size& size): 
    Texture(renderer, size.getWidth(), size.getHeight()) {}

Texture::Texture(Texture&& o): texture_(o.texture_), renderer_(o.renderer_), size_(o.size_), alpha_(o.alpha_) {
    o.texture_  = nullptr;
    o.renderer_ = {};
    o.alpha_    = false;
    o.size_     = Size{0, 0};
}


// Destructeur : libère la texture
Texture::~Texture() = default;

Texture& Texture::operator=(Texture&& o) noexcept {
    if (this != &o) {
        texture_   = o.texture_;
        renderer_  = o.renderer_;
        size_      = o.size_;
        alpha_     = o.alpha_;

        o.texture_  = nullptr;
        o.renderer_  = {};
        o.alpha_    = false;
        o.size_     = Size{0, 0};
    }
    return *this;
}

void Texture::colorize(const SDL_Color& color) {
    SDL_SetTextureColorMod(texture_.get(), color.r, color.g, color.b);
}

std::shared_ptr<Texture> Texture::copy() {
    std::shared_ptr<Texture> copy = std::make_shared<Texture>(renderer_, size_);
    if (alpha_) copy->convertAlpha();

    copy->fill(ColorUtils::TRANSPARENT);
    copy->blit(shared_from_this());

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
    return texture_.get();
}

void Texture::convertAlpha() {
    if (!alpha_ && SDL_SetTextureBlendMode(texture_.get(), SDL_BLENDMODE_BLEND) != 0)
        throw std::runtime_error("Failed to set blend mode of texture: " + std::string(SDL_GetError()));

    alpha_ = true;
}

void Texture::removeAlpha() {
    if (alpha_ && SDL_SetTextureBlendMode(texture_.get(), SDL_BLENDMODE_NONE) != 0)
        throw std::runtime_error("Failed to reset blend mode of texture: " + std::string(SDL_GetError()));

    alpha_ = false;
}

void Texture::fill(const SDL_Color& color) const {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    RenderTargetGuard target(renderer_, texture_);
    if (SDL_SetRenderDrawColor(lrenderer.get(), color.r, color.g, color.b, color.a) != 0 ||
        SDL_RenderClear(lrenderer.get()) != 0)
        throw std::runtime_error("Erreur lors du fill de la texture: " + std::string(SDL_GetError()));
}


void Texture::blit(const std::weak_ptr<BlitTarget>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    RenderTargetGuard target(renderer_, texture_);
    auto lsrc = src.lock();
    if (!lsrc) return;

    if (SDL_RenderCopy(lrenderer.get(), lsrc->get(), srcRect, destRect) != 0)
        throw std::runtime_error("Erreur lors du blitting de la texture: " + std::string(SDL_GetError()));
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src) const {
    blit(src, nullptr, nullptr);
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src, const Point& destPos) const {
    if (auto lsrc = src.lock()) {
        Rect destRect(destPos, lsrc->getSize());
        blit(src, nullptr, &destRect.get());
    }
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src, const Rect& destRect) const {
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Point& destPos) const {
    if (auto lsrc = src.lock()) {
        Rect destRect(destPos, lsrc->getSize());
        blit(src, &srcRect.get(), &destRect.get());
    }
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, &srcRect.get(), &destRect.get());
}

void Texture::blit(const std::weak_ptr<BlitTarget>& src, const Rect& srcRect, const Rect& destRect) const {
    blit(src, &srcRect.get(), &destRect.get());
}


void Texture::blit(const std::unique_ptr<BlitTarget>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    RenderTargetGuard target(renderer_, texture_);
    if (SDL_RenderCopy(lrenderer.get(), src->get(), srcRect, destRect) != 0)
        throw std::runtime_error("Erreur lors du blitting de la texture: " + std::string(SDL_GetError()));
}


void Texture::display(const Point& destPos) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    SDL_Rect destRect{destPos.getX(), destPos.getY(), getWidth(), getHeight()};
    if (SDL_RenderCopy(lrenderer.get(), texture_.get(), nullptr, &destRect) != 0)
        throw std::runtime_error("Erreur lors du display de la texture: " + std::string(SDL_GetError()));
}
