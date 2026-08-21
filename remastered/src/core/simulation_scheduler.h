#ifndef GEOCUBE_CORE_SIMULATION_SCHEDULER_H
#define GEOCUBE_CORE_SIMULATION_SCHEDULER_H

namespace geocube::core {

inline constexpr double kFixedStepSeconds = 1.0 / 60.0;
inline constexpr double kMaximumFrameSeconds = 0.25;
inline constexpr int kMaximumCatchUpSteps = 8;

struct SimulationSchedule {
  double frameSeconds = 0.0;
  int fixedSteps = 0;
  bool discardedAccumulator = false;
};

// Owns only wall-clock accumulation. Gameplay resources intentionally remain
// outside this class because they consume the complete clamped frame.
class SimulationScheduler final {
public:
  SimulationSchedule schedule(double elapsedSeconds);
  void reset() { m_accumulator = 0.0; }
  double accumulator() const { return m_accumulator; }

private:
  double m_accumulator = 0.0;
};

} // namespace geocube::core

#endif
