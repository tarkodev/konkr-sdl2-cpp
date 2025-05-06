#ifndef HEXAGONGRID_TPP
#define HEXAGONGRID_TPP

#include "Utils/HexagonUtils.hpp"
#include "Widgets/HexagonGrid.hpp"
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
void HexagonGrid<T>::addWidth(int delta)
{
    // Compute the new width, clamped to at least 2
    int newW = std::max(2, width_ + delta);
    if (newW == width_) return;

    // Build a new grid of size newW x height_, default‐initialized
    std::vector<T> newGrid(newW * height_, T());

    // Copy over existing cells (row by row)
    for (int row = 0; row < height_; ++row) {
        int oldRowStart = row * width_;
        int newRowStart = row * newW;
        int copyCols   = std::min(width_, newW);
        for (int col = 0; col < copyCols; ++col)
            newGrid[newRowStart + col] = std::move(grid_[oldRowStart + col]);
    }

    // Commit
    grid_.swap(newGrid);
    width_ = newW;
}

template<typename T>
void HexagonGrid<T>::addHeight(int delta)
{
    // Compute the new height, clamped to at least 2
    int newH = std::max(2, height_ + delta);
    if (newH == height_) return;

    // Build a new grid of size width_ x newH, default‐initialized
    std::vector<T> newGrid(width_ * newH, T());

    // Copy over existing rows (up to the smaller of old/new height)
    int copyRows = std::min(height_, newH);
    for (int row = 0; row < copyRows; ++row) {
        int oldRowStart = row * width_;
        int newRowStart = row * width_;
        std::move(
          grid_.begin()  + oldRowStart,
          grid_.begin()  + oldRowStart + width_,
          newGrid.begin() + newRowStart
        );
    }

    // Commit
    grid_.swap(newGrid);
    height_ = newH;
}


#endif
