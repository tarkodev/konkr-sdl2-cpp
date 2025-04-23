#ifndef POINT_HPP
#define POINT_HPP

#include "SDL.h"
#include <iostream>

/**
 * @brief Classe enveloppant SDL_Point.
 *
 * Fournit une interface orientée objet pour manipuler un point 2D
 * et permet la conversion explicite en SDL_Point.
 */
class Point {
public:
    // Constructeur par défaut
    Point() : point_{0, 0} {}
    
    // Constructeur avec coordonnées
    Point(int x, int y) : point_{x, y} {}
    
    // Constructeur à partir d'un SDL_Point
    Point(const SDL_Point& p) : point_(p) {}

    // Accesseurs
    int getX() const { return point_.x; }
    int getY() const { return point_.y; }

    // Mutateurs
    void setX(int x) { point_.x = x; }
    void setY(int y) { point_.y = y; }

    void addX(int x) { point_.x += x; }
    void addY(int y) { point_.y += y; }

    // Accès direct au SDL_Point interne
    const SDL_Point& get() const { return point_; }
    SDL_Point& get() { return point_; }

    // Opérateur de conversion explicite en SDL_Point pour utilisation via static_cast
    explicit operator SDL_Point() const {
        return point_;
    }

    // Opérateur d'addition : retourne une somme de points
    Point operator+(const Point& other) const {
        return Point(point_.x + other.point_.x, point_.y + other.point_.y);
    }

    // Opérateur de soustraction : retourne une soustraction de points
    Point operator-(const Point& other) const {
        return Point(point_.x - other.point_.x, point_.y - other.point_.y);
    }

    // Opérateur de multiplicaion : retourne un produit de points
    Point operator*(const double coef) const {
        return Point(point_.x * coef, point_.y * coef);
    }

    // Opérateur d'addition affecté
    Point& operator+=(const Point& other) {
        point_.x += other.point_.x;
        point_.y += other.point_.y;
        return *this;
    }

    // Opérateur de soustraction affecté
    Point& operator-=(const Point& other) {
        point_.x -= other.point_.x;
        point_.y -= other.point_.y;
        return *this;
    }

    // Opérateur de multiplication affecté
    Point& operator*=(const double coef) {
        point_.x *= coef;
        point_.y *= coef;
        return *this;
    }
    
    // Surcharge de l'opérateur << pour l'affichage
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "Point(" << p.point_.x << ", " << p.point_.y << ")";
        return os;
    }
    
private:
    SDL_Point point_;
};

#endif
