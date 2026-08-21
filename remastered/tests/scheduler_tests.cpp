#include "core/game_world.h"
#include "core/simulation_scheduler.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {

void expect(bool condition, const char* message)
{
  if (!condition) {
    std::cerr << message << '\n';
    std::exit(EXIT_FAILURE);
  }
}

void expectNear(double actual, double expected, const char* message)
{
  expect(std::fabs(actual - expected) < 0.000001, message);
}

} // namespace

int main()
{
  using namespace geocube::core;
  SimulationScheduler scheduler;

  expectNear(scheduler.schedule(-1.0).frameSeconds, 0.0,
             "negative time clamps to zero");
  expect(scheduler.schedule(kFixedStepSeconds * 0.5).fixedSteps == 0,
         "sub-step does not simulate");
  expect(scheduler.schedule(kFixedStepSeconds * 0.5).fixedSteps == 1,
         "accumulated sub-steps simulate once");

  scheduler.reset();
  const SimulationSchedule capped = scheduler.schedule(1.0);
  expectNear(capped.frameSeconds, kMaximumFrameSeconds,
             "large frame clamps to 250ms");
  expect(capped.fixedSteps == kMaximumCatchUpSteps,
         "large frame is capped at eight steps");
  expect(capped.discardedAccumulator, "excess accumulator is discarded");
  expect(scheduler.accumulator() < kFixedStepSeconds,
         "discarded schedule has no catch-up backlog");

  GameWorld world(99);
  world.startNewGame();
  world.stepFixed({});
  InputState shield;
  shield.setHeld(Action::Shield, true);
  world.advance(1.0, shield);
  expectNear(world.shieldRemainingSeconds(), kShieldMaximumSeconds - 0.25,
             "resources consume clamped frame duration");

  GameWorld pressedWorld(100);
  pressedWorld.startNewGame();
  pressedWorld.stepFixed({});
  InputState pause;
  pause.press(Action::Pause);
  pressedWorld.advance(kMaximumFrameSeconds, pause);
  expect(pressedWorld.state() == GameState::Paused,
         "pressed action is delivered during catch-up");

  std::cout << "Scheduler characterization tests passed\n";
}
