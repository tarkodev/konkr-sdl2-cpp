#include "Texture.hpp"
#include "ColorUtils.hpp"
#include "Point.hpp"
#include "RenderTargetGuard.hpp"
#include <sstream>
#include "Checker.hpp"

// Constructeur : charge la texture depuis le fichier
Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::string& file): renderer_(renderer) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");

    SDL_Texture* texture = IMG_LoadTexture(lrenderer.get(), file.c_str());
    SDL_Check(!texture, "IMG_LoadTexture");

    int w, h;
    SDL_Check(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h), "SDL_QueryTexture");

    texture_ = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);

    convertAlpha();
    size_ = {w, h};
}

Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, const std::shared_ptr<SDL_Texture>& texture): renderer_(renderer) {
    texture_ = texture;
    SDL_Check(!texture_, "Texture isn't defined.");

    int w, h;
    SDL_Check(SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h), "SDL_QueryTexture");

    convertAlpha();
    size_ = {w, h};
}

Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, int w, int h): renderer_(renderer) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) throw std::runtime_error("Renderer isn't initialized.");

    texture_ = std::shared_ptr<SDL_Texture>(SDL_CreateTexture(lrenderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h), SDL_DestroyTexture);
    SDL_Check(!texture_, "SDL_CreateTexture");

    convertAlpha();
    size_ = {w, h};
}

Texture::Texture(const std::weak_ptr<SDL_Renderer>& renderer, const Size& size): 
    Texture(renderer, size.getWidth(), size.getHeight()) {}

Texture::Texture(Texture&& o): texture_(o.texture_), renderer_(o.renderer_), size_(o.size_) {
    o.texture_  = nullptr;
    o.renderer_ = {};
    o.size_     = Size{0, 0};
}


// Destructeur : libère la texture
Texture::~Texture() = default;

Texture& Texture::operator=(Texture&& o) noexcept {
    if (this != &o) {
        texture_   = o.texture_;
        renderer_  = o.renderer_;
        size_      = o.size_;

        o.texture_  = nullptr;
        o.renderer_  = {};
        o.size_     = Size{0, 0};
    }
    return *this;
}

void Texture::colorize(const SDL_Color& color) {
    SDL_Check(SDL_SetTextureColorMod(texture_.get(), color.r, color.g, color.b), "SDL_SetTextureColorMod");
}

std::shared_ptr<Texture> Texture::copy() {
    std::shared_ptr<Texture> copy = std::make_shared<Texture>(renderer_, size_);
    copy->convertAlpha();

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
    SDL_Check(SDL_SetTextureBlendMode(texture_.get(), SDL_BLENDMODE_BLEND), "SDL_SetTextureBlendMode");
}

void Texture::removeAlpha() {
    SDL_Check(SDL_SetTextureBlendMode(texture_.get(), SDL_BLENDMODE_NONE), "SDL_SetTextureBlendMode");
}

void Texture::fill(const SDL_Color& color) const {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    RenderTargetGuard target(renderer_, texture_);
    SDL_Check(SDL_SetRenderDrawColor(lrenderer.get(), color.r, color.g, color.b, color.a), "SDL_SetRenderDrawColor");
    SDL_Check(SDL_RenderClear(lrenderer.get()), "SDL_RenderClear");
}


void Texture::blit(const std::weak_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    auto lrenderer = renderer_.lock();
    auto lsrc = src.lock();
    if (!lrenderer || !lsrc) return;

    lsrc->convertAlpha();
    RenderTargetGuard target(renderer_, texture_);
    SDL_Check(SDL_RenderCopy(lrenderer.get(), lsrc->get(), srcRect, destRect), "SDL_RenderCopy");
}

void Texture::blit(const std::weak_ptr<Texture>& src) const {
    blit(src, nullptr, nullptr);
}

void Texture::blit(const std::weak_ptr<Texture>& src, const Point& destPos) const {
    if (auto lsrc = src.lock()) {
        Rect destRect(destPos, lsrc->getSize());
        blit(src, nullptr, &destRect.get());
    }
}

void Texture::blit(const std::weak_ptr<Texture>& src, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(const std::weak_ptr<Texture>& src, const Rect& destRect) const {
    blit(src, nullptr, &destRect.get());
}

void Texture::blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Point& destPos) const {
    if (auto lsrc = src.lock()) {
        Rect destRect(destPos, lsrc->getSize());
        blit(src, &srcRect.get(), &destRect.get());
    }
}

void Texture::blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Size& destSize) const {
    Rect destRect({0, 0}, destSize);
    blit(src, &srcRect.get(), &destRect.get());
}

void Texture::blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Rect& destRect) const {
    blit(src, &srcRect.get(), &destRect.get());
}


void Texture::blit(const std::unique_ptr<Texture>& src, const SDL_Rect* srcRect, const SDL_Rect* destRect) const {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    src->convertAlpha();
    RenderTargetGuard target(renderer_, texture_);
    SDL_Check(SDL_RenderCopy(lrenderer.get(), src->get(), srcRect, destRect), "SDL_RenderCopy");
}


void Texture::display(const Point& destPos) {
    auto lrenderer = renderer_.lock();
    if (!lrenderer) return;

    SDL_Rect destRect{destPos.getX(), destPos.getY(), getWidth(), getHeight()};
    SDL_Check(SDL_RenderCopy(lrenderer.get(), texture_.get(), nullptr, &destRect), "SDL_RenderCopy");
}
