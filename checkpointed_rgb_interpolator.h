#ifndef CHECKPOINTED_RGB_INTERPOLATOR_H
#define CHECKPOINTED_RGB_INTERPOLATOR_H

#include "rgb.h"
#include "range.h"
#include "checkpointed_unit_interval.h"

class CheckpointedRGBInterpolator {
public:
  CheckpointedRGBInterpolator(const Range& src_range,
                              const CheckpointedUnitInterval<RGB>& rgb_checkpoints):
    src_range_(src_range), rgb_checkpoints_(rgb_checkpoints) {}

  void interpolate(double value, RGB* res) {
    CheckpointedUnitInterval<RGB>::RangeAndPayloads range_and_payloads;
    double normalized_value = src_range_.to0_1(value);
    rgb_checkpoints_.GetRangeAndPayloads(normalized_value, &range_and_payloads);
    RGBInterpolator interpolator(range_and_payloads.range,
                                 range_and_payloads.start_payload,
                                 range_and_payloads.end_payload);
    interpolator.interpolate(normalized_value, res);
  }

private:
  CheckpointedUnitInterval<RGB> rgb_checkpoints_;
  Range src_range_;
};

#endif
