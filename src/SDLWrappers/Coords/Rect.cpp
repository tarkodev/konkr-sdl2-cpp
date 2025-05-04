#include "SDL.h"
#include "SDLWrappers/Coords/Point.hpp"
#include "SDLWrappers/Coords/Size.hpp"
#include "SDLWrappers/Coords/Rect.hpp"
#include <iostream>
#include <algorithm>

Rect::Rect() : rect_{0, 0, 0, 0} {}
    
Rect::Rect(int x, int y, int w, int h) : rect_{x, y, w, h} {}

Rect::Rect(Point pos, Size size) : rect_{pos.getX(), pos.getY(), size.getWidth(), size.getHeight()} {}

Rect::Rect(const SDL_Rect& r) : rect_(r) {}

int Rect::getX() const { return rect_.x; }
int Rect::getY() const { return rect_.y; }
Point Rect::getPos() const { return Point{rect_.x, rect_.y}; }
int Rect::getWidth() const { return rect_.w; }
int Rect::getHeight() const { return rect_.h; }
Size Rect::getSize() const { return Size{rect_.w, rect_.h}; }

void Rect::setX(int x) { rect_.x = x; }
void Rect::setY(int y) { rect_.y = y; }
void Rect::setPos(Point pos) { rect_.x = pos.getX(); rect_.y = pos.getY(); }
void Rect::setWidth(int w) { rect_.w = w; }
void Rect::setHeight(int h) { rect_.h = h; }
void Rect::setPos(Size size) { rect_.w = size.getWidth(); rect_.h = size.getHeight(); }

void Rect::scale(double factor) {
    rect_.w = static_cast<int>(rect_.w * factor);
    rect_.h = static_cast<int>(rect_.h * factor);
}

const SDL_Rect& Rect::get() const { return rect_; }
SDL_Rect& Rect::get() { return rect_; }

Rect::operator SDL_Rect() const {
    return rect_;
}

Rect& Rect::operator+=(const Rect& other) {
    rect_.x += other.rect_.x;
    rect_.y += other.rect_.y;
    return *this;
}

bool Rect::contains(const int x, const int y) const {
    return x >= rect_.x && x < rect_.x + rect_.w &&
            y >= rect_.y && y < rect_.y + rect_.h;
}

bool Rect::contains(const Point& p) const {
    return p.getX() >= rect_.x && p.getX() < rect_.x + rect_.w &&
            p.getY() >= rect_.y && p.getY() < rect_.y + rect_.h;
}

bool Rect::contains(const Size& s) const {
    return s.getWidth() >= rect_.x && s.getWidth() < rect_.x + rect_.w &&
            s.getHeight() >= rect_.y && s.getHeight() < rect_.y + rect_.h;
}
