#ifndef HEXAGONGRID_HPP
#define HEXAGONGRID_HPP

#include <cmath>
#include <utility>
#include <vector>
#include <stdexcept>

/**
 * @brief Classe représentant une grille d'hexagones générique.
 *
 * La classe gère à la fois les dimensions et le stockage de la grille (dans un conteneur STL)
 * et fournit des fonctions de conversion entre les coordonnées hexagonales et les coordonnées en pixels.
 *
 * @tparam T Type des cellules.
 */
template<typename T>
class HexagonGrid {
public:
    using cell_type = T;

    /**
     * @brief Constructeur.
     * @param size Pair (width, height) de la grille.
     * @param defaultValue Valeur par défaut pour chaque cellule.
     */
    HexagonGrid(const std::pair<int, int>& size, const T& defaultValue = T());
    
    /**
     * @brief Constructeur.
     * @param gridFile Fichier de la grille.
     */
    HexagonGrid(const std::string gridFile);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    /**
     * @brief Récupère la valeur d'une cellule en coordonnées offset (x, y).
     * @param x Numéro de colonne.
     * @param y Numéro de ligne.
     * @return La valeur de la cellule.
     * @throws std::out_of_range si les indices sont invalides.
     */
    T get(int x, int y) const;

    /**
     * @brief Affecte une valeur à une cellule en coordonnées offset (x, y).
     * @param x Numéro de colonne.
     * @param y Numéro de ligne.
     * @param value Nouvelle valeur à affecter.
     * @throws std::out_of_range si les indices sont invalides.
     */
    void set(int x, int y, const T& value);

    /**
     * @brief Récupère la valeur d'une cellule en coordonnées axiales (q, r).
     * @param q Coordonnée axiale q.
     * @param r Coordonnée axiale r.
     * @return La valeur de la cellule.
     * @throws std::out_of_range si les indices sont invalides.
     */
    T getAxial(int q, int r) const;

    /**
     * @brief Affecte une valeur à une cellule en coordonnées axiales (q, r).
     * @param q Coordonnée axiale q.
     * @param r Coordonnée axiale r.
     * @param value Nouvelle valeur à affecter.
     * @throws std::out_of_range si les indices sont invalides.
     */
    void setAxial(int q, int r, const T& value);

private:
    int width_;
    int height_;
    std::vector<T> grid_;
};

#include "HexagonGrid.tpp"

#endif
