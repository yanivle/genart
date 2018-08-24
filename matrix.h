#ifndef MATRIX_H
#define MATRIX_H

template <class T>
class Matrix {
public:
  Matrix() : width_(0), height_(0) {}

  Matrix(int width, int height) :
    width_(width), height_(height), data_(width * height) {
  }

  void resize(int width, int height) {
    width_ = width;
    height_ = height;
    data_.resize(width * height);
  }

  int size() {
    return data_.size();
  }

  void clear() {
    std::fill(data_.begin(), data_.end(), 0);
  }

  T& at(int i) {
    return data_[i];
  }

  const T& at(int i) const {
    return data_[i];
  }

  T& at(int x, int y) {
    return data_[y * width_ + x];
  }

  const T& at(int x, int y) const {
    return data_[y * width_ + x];
  }

  bool inBounds(int x, int y) {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
  }

protected:
  int width_;
  int height_;
  std::vector<T> data_;
};

#endif
