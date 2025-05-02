#ifndef RECT_HPP
#define RECT_HPP

#include "SDL.h"
#include "Point.hpp"
#include "Size.hpp"
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
    Rect() : rect_{0, 0, 0, 0} {}
    
    // Constructeur avec paramètres individuels
    Rect(int x, int y, int w, int h) : rect_{x, y, w, h} {}

    // Constructeur avec points
    Rect(Point pos, Size size) : rect_{pos.getX(), pos.getY(), size.getWidth(), size.getHeight()} {}

    // Constructeur à partir d'un SDL_Rect
    Rect(const SDL_Rect& r) : rect_(r) {}

    // Accesseurs pour x, y, largeur et hauteur
    int getX() const { return rect_.x; }
    int getY() const { return rect_.y; }
    Point getPos() const { return Point{rect_.x, rect_.y}; }
    int getWidth() const { return rect_.w; }
    int getHeight() const { return rect_.h; }
    Size getSize() const { return Size{rect_.w, rect_.h}; }

    // Modificateurs
    void setX(int x) { rect_.x = x; }
    void setY(int y) { rect_.y = y; }
    void setPos(Point pos) { rect_.x = pos.getX(); rect_.y = pos.getY(); }
    void setWidth(int w) { rect_.w = w; }
    void setHeight(int h) { rect_.h = h; }
    void setPos(Size size) { rect_.w = size.getWidth(); rect_.h = size.getHeight(); }

    // Ajuste la taille en respectant un ratio (pour redimensionnement proportionnel)
    void scale(double factor) {
        rect_.w = static_cast<int>(rect_.w * factor);
        rect_.h = static_cast<int>(rect_.h * factor);
    }

    // Accès direct au SDL_Rect interne
    const SDL_Rect& get() const { return rect_; }
    SDL_Rect& get() { return rect_; }

    // Opérateur de conversion explicite en SDL_Rect pour utilisation via static_cast
    explicit operator SDL_Rect() const {
        return rect_;
    }

    // Opérateur d'affectation par addition (ex : translation)
    Rect& operator+=(const Rect& other) {
        rect_.x += other.rect_.x;
        rect_.y += other.rect_.y;
        return *this;
    }

    // Détermine si un point (x, y) est contenu dans le rectangle
    bool contains(int x, int y) const {
        return x >= rect_.x && x < rect_.x + rect_.w &&
               y >= rect_.y && y < rect_.y + rect_.h;
    }

    // Détermine si un point (x, y) est contenu dans le rectangle
    bool contains(Point& p) const {
        return p.getX() >= rect_.x && p.getX() < rect_.x + rect_.w &&
               p.getY() >= rect_.y && p.getY() < rect_.y + rect_.h;
    }

    // Détermine si un point (x, y) est contenu dans le rectangle
    bool contains(Size& s) const {
        return s.getWidth() >= rect_.x && s.getWidth() < rect_.x + rect_.w &&
               s.getHeight() >= rect_.y && s.getHeight() < rect_.y + rect_.h;
    }

    // Surcharge de l'opérateur << pour un affichage simple
    friend std::ostream& operator<<(std::ostream& os, const Rect& r) {
        os << "Rect(" << r.rect_.x << ", " << r.rect_.y 
           << ", " << r.rect_.w << ", " << r.rect_.h << ")";
        return os;
    }
    
private:
    SDL_Rect rect_;
};

#endif
