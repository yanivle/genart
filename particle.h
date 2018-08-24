#ifndef PARTICLE_H
#define PARTICLE_H

#include "point.h"

struct Particle {
  Particle(): position(0,0), velocity(0,0) {}
  Particle(Point position): position(position), velocity(0,0) {}
  Particle(Point position, Vec2 velocity):
    position(position), velocity(velocity) {}
    
  void advance(double speed, Vec2 accelaration) {
    accelaration *= speed;
    velocity += accelaration;
    Vec2 v = velocity;
    v *= speed;
    position += v;
  }

  Point position;
  Vec2 velocity;
};

#endif
