#include "SDL.h"
#include "Size.hpp"
#include "Point.hpp"

#include <iostream>

// Constructeur par défaut
Size::Size() : size_{0, 0} {}
    
// Constructeur avec coordonnées
Size::Size(int w, int h) : size_{w, h} {}

// Constructeur à partir d'un SDL_Point
Size::Size(const SDL_Point& p) : size_{p.x, p.y} {}

// Constructeur à partir d'un Size
Size::Size(const Size& s) : size_{s.getWidth(), s.getHeight()} {}

// Accesseurs
int Size::getWidth() const { return size_.x; }
int Size::getHeight() const { return size_.y; }

// Mutateurs
void Size::setWidth(int w) { size_.x = w; }
void Size::setHeight(int h) { size_.y = h; }

// Accès direct au SDL_Point interne
const SDL_Point& Size::get() const { return size_; }
SDL_Point& Size::get() { return size_; }

// Opérateur d'addition : retourne une somme de points
Size Size::operator+(const Size& other) const {
    return Size(size_.x + other.getWidth(), size_.y + other.getHeight());
}

// Opérateur d'addition : retourne une somme de points
Point Size::operator-(const Point& other) const {
    return Point{size_.x - other.getX(), size_.y - other.getY()};
}

// Opérateur de multiplicaion : retourne un produit de points
Size Size::operator*(const double coef) const {
    return Size(size_.x * coef, size_.y * coef);
}

// Opérateur de division(entière) : retourne une division entière de points
Size Size::operator/(const double coef) const {
    return Size{std::max(static_cast<int>(size_.x / coef), 0), std::max(static_cast<int>(size_.y / coef), 0)};
}

// Opérateur de division(entière) : retourne une division entière de points
Size Size::operator/(const Size& other) const {
    return Size{std::max(static_cast<int>(size_.x / other.getWidth()), 0), std::max(static_cast<int>(size_.y / other.getHeight()), 0)};
}

// Opérateur d'addition affecté
Size& Size::operator+=(const Size& other) {
    size_.x += other.getWidth();
    size_.y += other.getHeight();
    return *this;
}

// Opérateur de multiplication affecté
Size& Size::operator*=(const double coef) {
    size_.x *= coef;
    size_.y *= coef;
    return *this;
}

// Opérateur de division affecté
Size& Size::operator/=(const double coef) {
    size_.x = std::max(static_cast<int>(size_.x / coef), 0);
    size_.y = std::max(static_cast<int>(size_.y / coef), 0);
    return *this;
}
