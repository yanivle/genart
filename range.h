#ifndef RANGE
#define RANGE

#include <string>
#include <sstream>

struct Range {
  Range(double min, double max) : min(min), max(max) {}
  Range() {}

  double min;
  double max;

  bool contains(double value) const {
    return value >= min && value < max;
  }

  double to0_1(double value) const {
    return (value - min) / span();
  }

  double from0_1(double value) const {
    return (value * span()) + min;
  }

  double span() const {
    return max - min;
  }

  double random() const {
    return from0_1(double(rand()) / RAND_MAX);
  }

  std::string str() const {
    std::ostringstream ss;
    ss << "Range(" << min << ',' << max << ')';
    return ss.str();
  }
};

#endif
