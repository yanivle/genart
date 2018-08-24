#ifndef CHECKPOINTED_UNIT_INTERVAL_H
#define CHECKPOINTED_UNIT_INTERVAL_H

#include <vector>

#include "range.h"

template <class PAYLOAD_T>
class CheckpointedUnitInterval {
public:
  struct CheckpointWithPayload {
    CheckpointWithPayload(double checkpoint, const PAYLOAD_T& payload):
    checkpoint(checkpoint), payload(payload) {}
    double checkpoint;
    PAYLOAD_T payload;
  };

  void addCheckpoint(double checkpoint, const PAYLOAD_T& payload) {
    checkpoints_and_payloads_.push_back(CheckpointWithPayload(checkpoint, payload));
  }

  // Assumes checkpoints_and_payloads_ is not empty.
  int lowerBoundIndex(double value) {
    int i = 1;
    for (;i < checkpoints_and_payloads_.size(); ++i) {
      if (checkpoints_and_payloads_[i].checkpoint >= value) break;
    }
    return i - 1;
  }

  struct RangeAndPayloads {
    Range range;
    PAYLOAD_T start_payload;
    PAYLOAD_T end_payload;
  };
  // Assumes |value| is in a valid range.
  void GetRangeAndPayloads(double value, RangeAndPayloads* res) {
    int idx = lowerBoundIndex(value);
    res->start_payload = checkpoints_and_payloads_[idx].payload;
    res->end_payload = checkpoints_and_payloads_[idx + 1].payload;
    res->range = Range(checkpoints_and_payloads_[idx].checkpoint,
                       checkpoints_and_payloads_[idx + 1].checkpoint);
  }

private:
  std::vector<CheckpointWithPayload> checkpoints_and_payloads_;
};

#endif
