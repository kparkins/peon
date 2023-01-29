/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#ifndef PEON_COMMON_GRID_H
#define PEON_COMMON_GRID_H

#include <stddef.h>
#include <string.h>

namespace Peon {
template <typename T>
class Grid {
 public:
  Grid(size_t rows, size_t cols);
  ~Grid();

  T* operator[](int row);

 protected:
  T* mData;
  size_t mRows;
  size_t mCols;
};
}  // namespace Peon

template <typename T>
Peon::Grid<T>::Grid(size_t rows, size_t cols) : mRows(rows), mCols(cols) {
  mData = new T[rows * cols];
  memset(static_cast<void*>(&mData[0]), 0, mRows * mCols * sizeof(T));
}

template <typename T>
Peon::Grid<T>::~Grid() {
  delete[] mData;
}

template <typename T>
T* Peon::Grid<T>::operator[](int row) {
  return &mData[row * mRows];
}

#endif