#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <vector>

#include "point.h"
#include "image.h"
#include "integer_matrix.h"
#include "checkpointed_unit_interval.h"
#include "rgb_interpolator.h"
#include "particle.h"

void sierpinski(int iterations, IntegerMatrix* res) {
  res->resize(500, 500);

  Point p(50, 50);
  std::vector<Point> vertices;
  vertices.push_back(Point(20, 20));
  vertices.push_back(Point(30, 470));
  // vertices.push_back(Point(460, 30));
  vertices.push_back(Point(450, 480));
  for (int i = 0; i < iterations; ++i) {
    int coord = rand() % vertices.size();
    Point v = vertices[coord];
    v *= vertices.size() - 2;
    p += v;
    p *= 1.0 / (vertices.size() - 1);
    res->inc(p.x, p.y);
  }
}

int main(int argc, char* argv[]) {
  IntegerMatrix mat;

  sierpinski(1000000, &mat);

  CheckpointedUnitInterval<RGB> rgb_checkpoints;
  rgb_checkpoints.addCheckpoint(0, RGB(0, 0, 0));
  rgb_checkpoints.addCheckpoint(0.1, RGB(255, 0, 0));
  rgb_checkpoints.addCheckpoint(0.1, RGB(255, 0, 0));
  rgb_checkpoints.addCheckpoint(0.3, RGB(255, 255, 0));
  rgb_checkpoints.addCheckpoint(1.0, RGB(255, 255, 255));

  Image i;
  mat.toImage(rgb_checkpoints, &i);
  i.show("sierpinski.ppm");

  return 0;
}
