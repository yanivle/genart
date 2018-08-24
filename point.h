#ifndef POINT_H
#define POINT_H

#include "range.h"

struct Point {
  Point() : x(0), y(0) {}

  Point(double x, double y) : x(x), y(y) {}

  static Point random(const Range& range) {
    return Point(range.random(), range.random());
  }

  double dist_squared(const Point& other) const {
    return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
  }

  Point& operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }

  Point& operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Point operator-() const {
    return Point(-x, -y);
  }

  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }

  Point& operator*=(double a) {
    x *= a;
    y *= a;
    return *this;
  }

  Point operator*(double a) const {
    return Point(x * a, y * a);
  }

  Point rot90() const {
    return Point(y, -x);
  }

  double x;
  double y;
};

typedef Point Vec2;

#endif
