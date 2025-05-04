#ifndef POINT_HPP
#define POINT_HPP

#include "SDL.h"
#include <iostream>

class Size;

/**
 * @brief Classe enveloppant SDL_Point.
 *
 * Fournit une interface orientée objet pour manipuler un point 2D
 * et permet la conversion explicite en SDL_Point.
 */
class Point {
public:
    // Constructeur par défaut
    Point();
    
    // Constructeur avec coordonnées
    Point(int x, int y);
    
    // Constructeur à partir d'un SDL_Point
    Point(const SDL_Point& p);

    // Constructeur à partir d'une size
    Point(const Size& s);

    // Accesseurs
    const int getX() const;
    const int getY() const;

    // Mutateurs
    void setX(int x);
    void setY(int y);

    void addX(int x);
    void addY(int y);

    // Accès direct au SDL_Point interne
    const SDL_Point& get() const;
    SDL_Point& get();

    // Opérateur de conversion explicite en SDL_Point pour utilisation via static_cast
    explicit operator SDL_Point() const;

    // Opérateur d'addition : retourne une somme de points
    Point operator+(const Point& other) const;
    Point operator+(const Size& other) const;

    // Opérateur de soustraction : retourne une soustraction de points
    Point operator-(const Point& other) const;
    Point operator-(const Size& other) const;

    // Opérateur de multiplicaion : retourne un produit de points
    Point operator*(const double coef) const;

    // Opérateur de division(entière) : retourne une division entière de points
    Point operator/(const double coef) const;

    // Opérateur d'addition affecté
    Point& operator+=(const Point& other);
    Point& operator+=(const Size& other);

    // Opérateur de soustraction affecté
    Point& operator-=(const Point& other);
    Point& operator-=(const Size& other);

    // Opérateur de multiplication affecté
    Point& operator*=(const double coef);

    // Opérateur de division affecté
    Point& operator/=(const double coef);
    
private:
    SDL_Point point_;
};

#endif
