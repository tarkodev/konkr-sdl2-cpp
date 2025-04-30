#ifndef BLITTARGET_HPP
#define BLITTARGET_HPP

#include "SDL.h"
#include "Point.hpp"
#include "Size.hpp"
#include "Rect.hpp"

class BlitTarget {
public:
    explicit BlitTarget() = default;
    virtual ~BlitTarget() = default;

    virtual Size getSize() const = 0;
    virtual SDL_Texture* get() const = 0;

    virtual void blit(const BlitTarget* src) const = 0;
    virtual void blit(const BlitTarget* src, const Point& destPos) const = 0;
    virtual void blit(const BlitTarget* src, const Size& destSize) const = 0;
    virtual void blit(const BlitTarget* src, const Rect& destRect) const = 0;
    virtual void blit(const BlitTarget* src, const Rect& srcRect, const Point& destPos) const = 0;
    virtual void blit(const BlitTarget* src, const Rect& srcRect, const Size& destSize) const = 0;
    virtual void blit(const BlitTarget* src, const Rect& srcRect, const Rect& destRect) const = 0;
};

#endif
