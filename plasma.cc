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

// Folds src into res.
void squoosh(int dst_side, const IntegerMatrix& src, IntegerMatrix* dst) {
  dst->resize(dst_side, dst_side);
  for (int y = 0; y < dst_side; ++y) {
    for (int x = 0; x < dst_side; ++x) {
      dst->at(x, y) = (src.at(x / 2, y / 2) +
                       src.at(x / 2 + 1, y / 2) +
                       src.at(x / 2, y / 2 + 1) +
                       src.at(x / 2 + 1, y / 2 + 1)) / 4;
    }
  }
}

void renderPlasma(IntegerMatrix* res) {
  int pow = 9;
  int side = 1 << pow;
  IntegerMatrix base(side, side);

  Range rnd_range(0, 255);
  // Generate a random matrix
  for (int i = 0; i < res->size(); ++i) {
    base.at(i) = rnd_range.random();
  }

  *res = base;
  for (int scale_pow = 1; scale_pow < pow; ++scale_pow) {
    IntegerMatrix squooshed;
    squoosh(pow - scale_pow, base, &squooshed);
    squooshed.show();
    for (int y = 0; y < side; ++y) {
      for (int x = 0; x < side; ++x) {
        int org_x = x >> scale_pow;
        int org_y = y >> scale_pow;
        res->at(x, y) += squooshed.at(org_x, org_y);
      }
    }
    base = squooshed;
  }
}

int main(int argc, char* argv[]) {
  IntegerMatrix mat;
  Image i;

  CheckpointedUnitInterval<RGB> rgb_checkpoints;
  rgb_checkpoints.addCheckpoint(0, RGB(0, 0, 0));
  rgb_checkpoints.addCheckpoint(0.5, RGB(255, 255, 255));
  rgb_checkpoints.addCheckpoint(1.0, RGB(255, 0, 0));

  renderPlasma(&mat);
  mat.toImage(rgb_checkpoints, &i);
  i.show("plasma.ppm");

  return 0;
}
