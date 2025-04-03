#ifndef HEXAGONGRID_TPP
#define HEXAGONGRID_TPP

#include "HexagonUtils.hpp"
#include "HexagonGrid.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

template<typename T>
HexagonGrid<T>::HexagonGrid(const std::pair<int, int>& size, const T& defaultValue)
    : width_(size.first),
      height_(size.second),
      grid_(size.first * size.second, defaultValue)
{}

template<typename T>
HexagonGrid<T>::HexagonGrid(const std::string gridFile)
{
    width_ = 6;
    height_ = 6;
    grid_ = std::vector<T>(width_ * height_, T());
    //! A écrire
}

template<typename T>
T HexagonGrid<T>::get(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        throw std::out_of_range("Indice de cellule hors limite");
    return grid_[y * width_ + x];
}

template<typename T>
void HexagonGrid<T>::set(int x, int y, const T& value) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        throw std::out_of_range("Indice de cellule hors limite");
    grid_[y * width_ + x] = value;
}

template<typename T>
T HexagonGrid<T>::getAxial(int q, int r) const {
    auto [col, row] = HexagonUtils::axialToOffset(q, r);
    if (col < 0 || col >= width_ || row < 0 || row >= height_)
        throw std::out_of_range("Indice de cellule hors limite (axial)");
    return grid_[row * width_ + col];
}

template<typename T>
void HexagonGrid<T>::setAxial(int q, int r, const T& value) {
    auto [col, row] = HexagonUtils::axialToOffset(q, r);
    if (col < 0 || col >= width_ || row < 0 || row >= height_)
        throw std::out_of_range("Indice de cellule hors limite (axial)");
    grid_[row * width_ + col] = value;
}

#endif
