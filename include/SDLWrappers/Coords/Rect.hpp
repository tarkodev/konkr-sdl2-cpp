#ifndef RECT_HPP
#define RECT_HPP

#include "SDL.h"
#include "SDLWrappers/Coords/Point.hpp"
#include "SDLWrappers/Coords/Size.hpp"
#include <iostream>
#include <algorithm>

/**
 * @brief Classe enveloppant SDL_Rect.
 *
 * Fournit une interface orientée objet pour manipuler un rectangle
 * et permet la conversion explicite en SDL_Rect.
 */
class Rect {
public:
    // Constructeur par défaut
    Rect();
    
    // Constructeur avec paramètres individuels
    Rect(int x, int y, int w, int h);

    // Constructeur avec points
    Rect(Point pos, Size size);

    // Constructeur à partir d'un SDL_Rect
    Rect(const SDL_Rect& r);

    // Accesseurs pour x, y, largeur et hauteur
    int getX() const;
    int getY() const;
    Point getPos() const;
    int getWidth() const;
    int getHeight() const;
    Size getSize() const;

    // Modificateurs
    void setX(int x);
    void setY(int y);
    void setPos(Point pos);
    void setWidth(int w);
    void setHeight(int h);
    void setPos(Size size);

    // Ajuste la taille en respectant un ratio (pour redimensionnement proportionnel)
    void scale(double factor);

    // Accès direct au SDL_Rect interne
    const SDL_Rect& get() const;
    SDL_Rect& get();

    // Opérateur de conversion explicite en SDL_Rect pour utilisation via static_cast
    explicit operator SDL_Rect() const;

    // Opérateur d'affectation par addition (ex : translation)
    Rect& operator+=(const Rect& other);

    // Détermine si un point (x, y) est contenu dans le rectangle
    bool contains(const int x, const int y) const;
    bool contains(const Point& p) const;
    bool contains(const Size& s) const;
    
private:
    SDL_Rect rect_;
};

#endif
