#ifndef INTEGER_MATRIX
#define INTEGER_MATRIX

#include "image.h"
#include "rgb_interpolator.h"
#include "checkpointed_unit_interval.h"
#include "checkpointed_rgb_interpolator.h"

class IntegerMatrix : public Matrix<int> {
public:
  IntegerMatrix() {}

  IntegerMatrix(int width, int height) :
    Matrix<int>(width, height) {
      std::fill(data_.begin(), data_.end(), 0);
  }

  void inc(int x, int y, int amount = 1) {
    at(x, y) += amount;
  }

  int max() {
    int max = 0;
    for (int i = 0; i < data_.size(); ++i) {
      if (at(i) > max) {
        max = at(i);
      }
    }
    return max;
  }

  void toBitImage(Image* res) {
    res->resize(width_, height_);
    for (int i = 0; i < data_.size(); ++i) {
      RGB& rgb = res->at(i);
      if (at(i)) {
        rgb.r = rgb.g = rgb.b = 255;
      } else {
        rgb.r = rgb.g = rgb.b = 0;
      }
    }
  }

  void toImage(Image* res) {
    res->resize(width_, height_);
    int mx = max();
    for (int i = 0; i < data_.size(); ++i) {
      RGB& rgb = res->at(i);
      rgb.r = rgb.g = rgb.b = 255 * at(i) / mx;
    }
  }

  void toImage(const RGB& mn, const RGB& mx, Image* res) {
    RGBInterpolator interpolator(Range(0, max()), mn, mx);
    res->resize(width_, height_);
    for (int i = 0; i < data_.size(); ++i) {
      interpolator.interpolate(at(i), &res->at(i));
    }
  }

  void toImage(const CheckpointedUnitInterval<RGB>& checkpointed_rgb, Image* res) {
    CheckpointedRGBInterpolator interpolator(Range(0, max()), checkpointed_rgb);
    res->resize(width_, height_);
    for (int i = 0; i < data_.size(); ++i) {
      interpolator.interpolate(at(i), &res->at(i));
    }
  }
};

#endif
