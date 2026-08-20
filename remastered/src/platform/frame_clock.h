#ifndef GEOCUBE_REMASTERED_FRAME_CLOCK_H
#define GEOCUBE_REMASTERED_FRAME_CLOCK_H

#include <chrono>

namespace geocube::platform {

class FrameClock final {
public:
  FrameClock();

  void reset();
  double tick();

private:
  using Clock = std::chrono::steady_clock;
  Clock::time_point m_last;
};

} // namespace geocube::platform

#endif
