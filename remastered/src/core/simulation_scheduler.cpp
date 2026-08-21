#include "core/simulation_scheduler.h"

#include <algorithm>

namespace geocube::core {

SimulationSchedule SimulationScheduler::schedule(double elapsedSeconds)
{
  SimulationSchedule result;
  result.frameSeconds =
      std::clamp(elapsedSeconds, 0.0, kMaximumFrameSeconds);
  m_accumulator += result.frameSeconds;

  while (m_accumulator >= kFixedStepSeconds &&
         result.fixedSteps < kMaximumCatchUpSteps) {
    m_accumulator -= kFixedStepSeconds;
    ++result.fixedSteps;
  }

  if (result.fixedSteps == kMaximumCatchUpSteps &&
      m_accumulator >= kFixedStepSeconds) {
    m_accumulator = 0.0;
    result.discardedAccumulator = true;
  }
  return result;
}

} // namespace geocube::core
