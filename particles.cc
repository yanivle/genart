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

enum AttractionMode {
  ATTRACTION_MODE_GRAVITY = 0,
  ATTRACTION_MODE_SPRING = 1
};

struct ParticlesSettings {
  ParticlesSettings(int iterations, double speed, AttractionMode mode, int num_particles,
                    bool randomize_particles, int particle_size, bool random_movement) :
                    iterations(iterations), speed(speed), mode(mode),
                    num_particles(num_particles), randomize_particles(randomize_particles),
                    particle_size(particle_size), random_movement(random_movement) {}
  int iterations;
  double speed;
  AttractionMode mode;
  int num_particles;
  bool randomize_particles;
  int particle_size;
  bool random_movement;
};

void simulateParticles(const ParticlesSettings& settings, IntegerMatrix* res) {
  res->resize(500, 500);

  std::vector<Particle> particles;
  if (settings.randomize_particles) {
    Range position_range(0, 500);
    Range velocity_range(-50, 50);
    for (int j = 0; j < settings.num_particles; ++j) {
      particles.push_back(Particle(Point::random(position_range), Vec2::random(velocity_range)));
    }
  } else {
    Point center(250, 250);
    int radius = 200;
    double alpha = 2 * M_PI / settings.num_particles;
    for (int j = 0; j < settings.num_particles; ++j) {
      Point p(sin(M_PI / 4 + alpha * j), cos(M_PI / 4 + alpha * j));
      Vec2 v = (p * 100).rot90();
      p *= radius;
      p += center;
      particles.push_back(Particle(p, v));
    }
  }

  for (int i = 0; i < settings.iterations; ++i) {
    for (int j = 0; j < settings.num_particles; ++j) {
      Vec2 accelaration(0, 0);
      for (int k = 0; k < settings.num_particles; ++k) {
        if (k != j) {
          Vec2 accelaration_component = particles[k].position - particles[j].position;
          if (settings.mode == ATTRACTION_MODE_GRAVITY) {
            double dist_squared = particles[k].position.dist_squared(particles[j].position);
            accelaration_component *= (1000 / dist_squared);
          }
          if (settings.random_movement) {
            Range random_movement_range(-1000, 1000);
            accelaration_component += Vec2::random(random_movement_range);
          }
          accelaration += accelaration_component;
        }
      }
      particles[j].advance(settings.speed, accelaration);
      int x = particles[j].position.x;
      int y = particles[j].position.y;
      int width = settings.particle_size;
      int height = settings.particle_size;
      for (int m = 0; m < width; ++m) {
        for (int l = 0; l < height; ++l) {
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
  Image i;

  CheckpointedUnitInterval<RGB> rgb_checkpoints;
  rgb_checkpoints.addCheckpoint(0, RGB(0, 0, 0));
  rgb_checkpoints.addCheckpoint(0.1, RGB(255, 0, 0));
  rgb_checkpoints.addCheckpoint(0.3, RGB(255, 255, 0));
  rgb_checkpoints.addCheckpoint(1.0, RGB(255, 255, 255));

  ParticlesSettings settings(1000000,  // iterations
                             0.00001,  // speed
                             ATTRACTION_MODE_SPRING,
                             3,      // num_particles
                             false,  // randomize_particles
                             10,     // particle_size
                             false);  // random_movement

  simulateParticles(settings, &mat);
  mat.toImage(rgb_checkpoints, &i);
  i.show("3body.ppm");

  mat.clear();
  settings.mode = ATTRACTION_MODE_GRAVITY;
  settings.randomize_particles = true;
  settings.num_particles = 5;
  simulateParticles(settings, &mat);
  mat.toImage(rgb_checkpoints, &i);
  i.show("very_height.ppm");

  return 0;
}
