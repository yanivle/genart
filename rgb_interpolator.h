#ifndef RGB_INTERPOLATOR_H
#define RGB_INTERPOLATOR_H

#include "range.h"
#include "rgb.h"

class RGBInterpolator {
public:
  RGBInterpolator(const Range& range, const RGB& start, const RGB& end):
  range_(range), start_(start), end_(end) {}

  void interpolate(double value, RGB* res) {
    double x = range_.to0_1(value);
    res->r = start_.r * (1 - x) + end_.r * x;
    res->g = start_.g * (1 - x) + end_.g * x;
    res->b = start_.b * (1 - x) + end_.b * x;
  }

private:
  Range range_;
  RGB start_;
  RGB end_;
};

#endif
