#include "platform/frame_clock.h"

#include <algorithm>

namespace geocube::platform {

FrameClock::FrameClock() : m_last(Clock::now())
{
}

void FrameClock::reset()
{
  m_last = Clock::now();
}

double FrameClock::tick()
{
  const Clock::time_point now = Clock::now();
  const std::chrono::duration<double> elapsed = now - m_last;
  m_last = now;

  // Focus changes and debugger stops must not create a huge simulation step.
  return std::clamp(elapsed.count(), 0.0, 0.25);
}

} // namespace geocube::platform
