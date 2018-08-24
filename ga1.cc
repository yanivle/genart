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

enum AttractionMode {
  GRAVITY = 0,
  SPRING = 1
};
void simulateParticles(int iterations, double speed, AttractionMode mode, IntegerMatrix* res) {
  res->resize(500, 500);

  std::vector<Particle> particles;
  Range position_range(0, 500);
  Range velocity_range(-10, 10);
  // const int NUM_PARTICLES = 20;
  const int NUM_PARTICLES = 2;
  // for (int j = 0; j < NUM_PARTICLES; ++j) {
  //   particles.push_back(Particle(Point::random(position_range), Vec2::random(velocity_range)));
  // }
  particles.push_back(Particle(Point(50, 50), Vec2(150, 0)));
  particles.push_back(Particle(Point(450, 450), Vec2(-150, 0)));

  for (int i = 0; i < iterations; ++i) {
    for (int j = 0; j < NUM_PARTICLES; ++j) {
      Vec2 accelaration(0, 0);
      for (int k = 0; k < NUM_PARTICLES; ++k) {
        if (k != j) {
          double dist_squared = particles[k].position.dist_squared(particles[j].position);
          Vec2 accelaration_component = particles[k].position - particles[j].position;
          if (mode == GRAVITY) {
            accelaration_component *= (1000 / dist_squared);
          }
          accelaration += accelaration_component;
        }
      }
      particles[j].advance(speed, accelaration);
      int x = particles[j].position.x;
      int y = particles[j].position.y;
      int height = 10 + sin(i * speed) * 50;
      int width = 10;// + cos(i * speed) * 50;
      for (int l = 0; l < height; ++l) {
        for (int m = 0; m < width; ++m) {
          if (res->inBounds(x + m, y + l)) {
            res->inc(x + m, y + l);
          }
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  IntegerMatrix mat;

  // sierpinski(1000000, &mat);
  // if (argc != 2) return -1;
  // simulateParticles(1000000, atof(argv[1]), SPRING, &mat);

  CheckpointedUnitInterval<RGB> rgb_checkpoints;
  const RGB spectrum[] = {
    RGB(0, 0, 0),
    RGB(148, 0, 211),
    RGB(75, 0, 130),
    RGB(0, 0, 255),
    RGB(0, 255, 0),
    RGB(255, 255, 0),
    RGB(255, 127, 0),
    RGB(255, 0 , 0),
    RGB(255, 255 , 255),
  };
  int num_colors = sizeof(spectrum) / sizeof(RGB);
  for (int i = 0; i < num_colors; ++i) {
    rgb_checkpoints.addCheckpoint(double(i) / (num_colors - 1), spectrum[i]);
  }
  // rgb_checkpoints.addCheckpoint(0, RGB(0, 0, 0));
  // rgb_checkpoints.addCheckpoint(0.1, RGB(255, 0, 0));
  // rgb_checkpoints.addCheckpoint(0.1, RGB(255, 0, 0));
  // rgb_checkpoints.addCheckpoint(0.3, RGB(255, 255, 0));
  // rgb_checkpoints.addCheckpoint(1.0, RGB(255, 255, 255));

  Image i;
  // mat.toImage(RGB(0, 0, 0), RGB(255, 0, 0), &i);
  mat.toImage(rgb_checkpoints, &i);
  // mat.toBitImage(&i);
  // i.save_ppm("test.ppm");
  i.show();

  // Image i(500, 500);
  // RGB *rgb = &i.at(0,0);
  // for (int y = 0; y < 500; ++y) {
  //   for (int x = 0; x < 500; ++x) {
  //     rgb->r = x % 256;
  //     rgb->g = 255;//y % 255;
  //     rgb->b = 255;//(x + y) % 255;
  //     rgb++;
  //   }
  // }
  // i.save_ppm("test.ppm");
  // i.show();
  return 0;
}
