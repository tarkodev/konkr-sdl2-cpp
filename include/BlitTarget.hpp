#ifndef BLITTARGET_HPP
#define BLITTARGET_HPP

#include "SDL.h"
#include "SDLWrappers/Coords/Point.hpp"
#include "SDLWrappers/Coords/Size.hpp"
#include "SDLWrappers/Coords/Rect.hpp"
#include <memory>

class Texture;

class BlitTarget {
public:
    explicit BlitTarget() = default;
    virtual ~BlitTarget() = default;

    virtual Size getSize() const = 0;
    virtual SDL_Texture* get() const = 0;

    virtual void blit(const std::weak_ptr<Texture>& src) const = 0;
    virtual void blit(const std::weak_ptr<Texture>& src, const Point& destPos) const = 0;
    virtual void blit(const std::weak_ptr<Texture>& src, const Size& destSize) const = 0;
    virtual void blit(const std::weak_ptr<Texture>& src, const Rect& destRect) const = 0;
    virtual void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Point& destPos) const = 0;
    virtual void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Size& destSize) const = 0;
    virtual void blit(const std::weak_ptr<Texture>& src, const Rect& srcRect, const Rect& destRect) const = 0;
};

#endif
