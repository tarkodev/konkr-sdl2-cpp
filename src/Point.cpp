#include "SDL.h"
#include "Point.hpp"
#include "Size.hpp"

#include <iostream>

// Constructeur par défaut
Point::Point() : point_{0, 0} {}

// Constructeur avec coordonnées
Point::Point(int x, int y) : point_{x, y} {}

// Constructeur à partir d'un SDL_Point
Point::Point(const SDL_Point& p) : point_(p) {}

// Constructeur à partir d'une Size
Point::Point(const Size& s) : point_{s.getWidth(), s.getHeight()} {}

// Accesseurs
int Point::getX() const { return point_.x; }
int Point::getY() const { return point_.y; }

// Mutateurs
void Point::setX(int x) { point_.x = x; }
void Point::setY(int y) { point_.y = y; }

void Point::addX(int x) { point_.x += x; }
void Point::addY(int y) { point_.y += y; }

// Accès direct au SDL_Point interne
const SDL_Point& Point::get() const { return point_; }
SDL_Point& Point::get() { return point_; }

// Opérateur de conversion explicite en SDL_Point pour utilisation via static_cast
Point::operator SDL_Point() const {
    return point_;
}

// Opérateur d'addition : retourne une somme de points
Point Point::operator+(const Point& other) const {
    return Point(point_.x + other.point_.x, point_.y + other.point_.y);
}

// Opérateur d'addition : retourne une somme de points
Point Point::operator+(const Size& other) const {
    return Point(point_.x + other.getWidth(), point_.y + other.getHeight());
}

// Opérateur de soustraction : retourne une soustraction de points
Point Point::operator-(const Point& other) const {
    return Point(point_.x - other.point_.x, point_.y - other.point_.y);
}

// Opérateur de soustraction : retourne une soustraction de points
Point Point::operator-(const Size& other) const {
    return Point(point_.x - other.getWidth(), point_.y - other.getHeight());
}

// Opérateur de multiplicaion : retourne un produit de points
Point Point::operator*(const double coef) const {
    return Point(static_cast<int>(point_.x * coef), static_cast<int>(point_.y * coef));
}

// Opérateur de division(entière) : retourne une division entière de points
Point Point::operator/(const double coef) const {
    return Point(static_cast<int>(point_.x / coef), static_cast<int>(point_.y / coef));
}

// Opérateur d'addition affecté
Point& Point::operator+=(const Point& other) {
    point_.x += other.point_.x;
    point_.y += other.point_.y;
    return *this;
}

// Opérateur d'addition affecté
Point& Point::operator+=(const Size& other) {
    point_.x += other.getWidth();
    point_.y += other.getHeight();
    return *this;
}

// Opérateur de soustraction affecté
Point& Point::operator-=(const Point& other) {
    point_.x -= other.point_.x;
    point_.y -= other.point_.y;
    return *this;
}

// Opérateur de soustraction affecté
Point& Point::operator-=(const Size& other) {
    point_.x -= other.getWidth();
    point_.y -= other.getHeight();
    return *this;
}

// Opérateur de multiplication affecté
Point& Point::operator*=(const double coef) {
    point_.x = static_cast<int>(point_.x * coef);
    point_.y = static_cast<int>(point_.y * coef);
    return *this;
}

// Opérateur de division affecté
Point& Point::operator/=(const double coef) {
    point_.x = static_cast<int>(point_.x / coef);
    point_.y = static_cast<int>(point_.y / coef);
    return *this;
}
