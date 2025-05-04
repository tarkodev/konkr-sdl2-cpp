#ifndef SIZE_HPP
#define SIZE_HPP

#include "SDL.h"
#include "SDLWrappers/Coords/Point.hpp"

#include <algorithm>

/**
 * @brief Classe Size représentant une taille avec une largeur et une hauteur.
 */
class Size {
public:
    // Constructeur par défaut
    Size();
    
    // Constructeur avec coordonnées
    Size(int w, int h);
    
    // Constructeur à partir d'un SDL_Point
    Size(const SDL_Point& p);

    // Constructeur à partir d'un Size
    Size(const Size& s);

    // Constructeur à partir d'un Point
    Size(const Point& p);

    // Accesseurs
    int getWidth() const;
    int getHeight() const;

    // Mutateurs
    void setWidth(int w);
    void setHeight(int h);

    // Accès direct au SDL_Point interne
    const SDL_Point& get() const;
    SDL_Point& get();

    // Opérateur d'addition : retourne une somme de points
    Size operator+(const Size& other) const;
    Point operator+(const Point& other) const;

    // Opérateur d'addition : retourne une somme de points
    Point operator-(const Point& other) const;

    // Opérateur de multiplicaion : retourne un produit de points
    Size operator*(const double coef) const;

    // Opérateur de division(entière) : retourne une division entière de taille
    Size operator/(const double coef) const;

    // Opérateur de division(entière) : retourne une division entière de taille
    Size operator/(const Size& other) const;

    // Opérateur d'addition affecté
    Size& operator+=(const Size& other);

    // Opérateur de multiplication affecté
    Size& operator*=(const double coef);

    // Opérateur de division affecté
    Size& operator/=(const double coef);
    
private:
    SDL_Point size_;
};

#endif
