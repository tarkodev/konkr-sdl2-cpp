#ifndef SIZE_HPP
#define SIZE_HPP

#include "Point.hpp"

/**
 * @brief Classe Size représentant une taille avec une largeur et une hauteur.
 */
class Size {
public:
    // Constructeur par défaut
    Size() : size_{0, 0} {}
    
    // Constructeur avec coordonnées
    Size(int w, int h) : size_{w, h} {}
    
    // Constructeur à partir d'un SDL_Point
    Size(const SDL_Point& p) : size_{p.x, p.y} {}

    // Constructeur à partir d'un Size
    Size(const Size& s) : size_{s.getWidth(), s.getHeight()} {}

    // Accesseurs
    int getWidth() const { return size_.x; }
    int getHeight() const { return size_.y; }

    // Mutateurs
    void setWidth(int w) { size_.x = w; }
    void setHeight(int h) { size_.y = h; }

    // Accès direct au SDL_Point interne
    const SDL_Point& get() const { return size_; }
    SDL_Point& get() { return size_; }

    // Opérateur d'addition : retourne une somme de points
    Size operator+(const Size& other) const {
        return Size(size_.x + other.getWidth(), size_.y + other.getHeight());
    }

    // Opérateur de multiplicaion : retourne un produit de points
    Size operator*(const double coef) const {
        return Size(size_.x * coef, size_.y * coef);
    }

    // Opérateur d'addition affecté
    Size& operator+=(const Size& other) {
        size_.x += other.getWidth();
        size_.y += other.getHeight();
        return *this;
    }

    // Opérateur de multiplication affecté
    Size& operator*=(const double coef) {
        size_.x *= coef;
        size_.y *= coef;
        return *this;
    }
    
    // Surcharge de l'opérateur << pour l'affichage
    friend std::ostream& operator<<(std::ostream& os, const Size& p) {
        os << "Size(" << p.getWidth() << ", " << p.getHeight() << ")";
        return os;
    }
    
private:
    SDL_Point size_;
};

#endif
