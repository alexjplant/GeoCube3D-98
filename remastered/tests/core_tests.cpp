#include "core/collision.h"
#include "core/game_world.h"
#include "core/high_scores.h"
#include "core/math.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {

int failures = 0;

void expect(bool condition, const char* expression, const char* test)
{
  if (condition)
    return;
  ++failures;
  std::cerr << test << ": failed: " << expression << '\n';
}

void expectNear(float actual, float expected, const char* expression,
                const char* test)
{
  expect(std::fabs(actual - expected) <= 0.0001f, expression, test);
}

void testVectorMath()
{
  constexpr const char* test = "vector math";
  using namespace geocube::core;
  const Vec3 first{1.0f, 2.0f, 3.0f};
  const Vec3 second{4.0f, -2.0f, 1.0f};
  expect((first + second) == Vec3{5.0f, 0.0f, 4.0f},
         "vector addition", test);
  expectNear(dot(first, second), 3.0f, "dot(first, second)", test);
  expect(cross({1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}) ==
             Vec3{0.0f, 0.0f, 1.0f},
         "cross product", test);
  expectNear(length({3.0f, 4.0f, 0.0f}), 5.0f, "length", test);
  expect(nearlyEqual(normalized({0.0f, 3.0f, 4.0f}), {0.0f, 0.6f, 0.8f}),
         "normalization", test);
  expect(nearlyEqual(reflect({1.0f, -2.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
                     {1.0f, 2.0f, 0.0f}),
         "reflection", test);
}

void testCollisionPrimitives()
{
  constexpr const char* test = "collision primitives";
  using namespace geocube::core;
  Vec3 position{999.0f, 0.0f, 0.0f};
  Vec3 velocity{120.0f, 0.0f, 0.0f};
  expect(advanceAndReflect(position, velocity, 0.1f), "border collision",
         test);
  expectNear(position.x, 1000.0f, "border position", test);
  expectNear(velocity.x, -120.0f, "border velocity reflection", test);
  expect(spheresIntersect({}, 2.0f, {3.0f, 0.0f, 0.0f}, 1.0f),
         "sphere intersection", test);
  expect(cylindersIntersect({{}, 2.0f, 1.0f},
                            {{1.5f, 1.0f, 0.0f}, 2.0f, 1.0f}),
         "cylinder intersection", test);
  expect(pointInAabb({1.0f, 1.0f, 1.0f}, {{0.0f, 0.0f, 0.0f},
                                           {2.0f, 2.0f, 2.0f}}),
         "AABB containment", test);
  expect(segmentIntersectsSphere({0.0f, 0.0f, 0.0f},
                                 {10.0f, 0.0f, 0.0f},
                                 {5.0f, 1.0f, 0.0f}, 1.0f),
         "swept sphere intersection", test);
}

void prepareWorld(geocube::core::GameWorld& world)
{
  world.startNewGame("Test Pilot");
  world.stepFixed({});
  world.clearRocks();
  world.clearBullets();
  world.setPlayerPosition({-900.0f, -900.0f, -900.0f});
}

void testLevelsAndSplitting()
{
  constexpr const char* test = "levels and rock splitting";
  using namespace geocube::core;
  GameWorld world(42);
  world.startNewGame("Test Pilot");
  world.stepFixed({});
  expect(world.state() == GameState::Running, "loading to running", test);
  expect(world.rocks().size() == 1, "level one rock count", test);
  expect(world.currentLevel().largeType == RockType::Cube,
         "level one rock type", test);
  expect(world.lives() == 3, "starting lives", test);
  expect(world.levelNumber() == 1, "starting level number", test);

  GameWorld selectedLevel(43);
  selectedLevel.startNewGame("Test Pilot", 3);
  selectedLevel.stepFixed({});
  expect(selectedLevel.levelIndex() == 3 && selectedLevel.levelNumber() == 4,
         "selected starting level", test);

  world.clearRocks();
  world.setPlayerPosition({-900.0f, -900.0f, -900.0f});
  world.spawnRock(RockSize::Large, RockType::Cube, {0.0f, 0.0f, 20.0f});
  world.spawnBullet({}, {0.0f, 0.0f, 600.0f});
  world.stepFixed({});
  expect(world.score() == 50, "large rock score", test);
  expect(world.soundEventPending(SoundEvent::Hit),
         "rock hit sound event", test);
  world.clearSoundEvents();
  expect(world.rocks().size() == 4, "large rock splits into four", test);
  expect(world.rocks()[0].size == RockSize::Medium,
         "large rock child size", test);

  world.clearRocks();
  world.clearBullets();
  world.spawnRock(RockSize::Medium, RockType::Cube, {0.0f, 0.0f, 20.0f});
  world.spawnBullet({}, {0.0f, 0.0f, 600.0f});
  world.stepFixed({});
  expect(world.score() == 150, "medium rock score", test);
  expect(world.rocks().size() == 2, "medium rock splits into two", test);
  expect(world.rocks()[0].size == RockSize::Small,
         "medium rock child size", test);

  world.clearRocks();
  world.clearBullets();
  world.spawnRock(RockSize::Small, RockType::Cube, {0.0f, 0.0f, 20.0f});
  world.spawnBullet({}, {0.0f, 0.0f, 600.0f});
  world.stepFixed({});
  expect(world.score() == 350, "small rock score", test);
  expect(world.levelIndex() == 1, "empty level advances", test);
}

void testLevelWrapping()
{
  constexpr const char* test = "level wrapping";
  using namespace geocube::core;
  GameWorld world(7);
  world.startNewGame();
  world.stepFixed({});
  for (int level = 0; level < 5; ++level) {
    world.clearRocks();
    world.stepFixed({});
  }
  expect(world.levelIndex() == 0, "wrapped level index", test);
  expect(world.levelWrap() == 1, "wrapped level count", test);
  expect(world.levelNumber() == 6, "wrapped display level", test);
}

void testStateAndLives()
{
  constexpr const char* test = "states and lives";
  using namespace geocube::core;
  GameWorld world(9);
  prepareWorld(world);
  world.spawnRock(RockSize::Small, RockType::Cube,
                  {-900.0f, -900.0f, -900.0f});
  world.stepFixed({});
  expect(world.state() == GameState::PlayerHit, "player hit state", test);
  expect(world.soundEventPending(SoundEvent::PlayerHit),
         "player hit sound event", test);
  world.clearSoundEvents();
  for (int step = 0; step < 480; ++step)
    world.stepFixed({});
  expect(world.lives() == 2, "life decrement", test);
  expect(world.state() == GameState::Running, "life reset state", test);

  for (int life = 0; life < 2; ++life) {
    world.clearRocks();
    world.setPlayerPosition({-900.0f, -900.0f, -900.0f});
    world.spawnRock(RockSize::Small, RockType::Cube,
                    {-900.0f, -900.0f, -900.0f});
    world.stepFixed({});
    for (int step = 0; step < 480; ++step)
      world.stepFixed({});
  }
  expect(world.lives() == 0, "game over life count", test);
  expect(world.state() == GameState::GameOver, "game over state", test);

  GameWorld shielded(10);
  prepareWorld(shielded);
  InputState shield;
  shield.setHeld(Action::Shield, true);
  shielded.spawnRock(RockSize::Small, RockType::Cube,
                     {-900.0f, -900.0f, -900.0f});
  shielded.stepFixed(shield);
  expect(shielded.state() == GameState::Running &&
             shielded.soundEventPending(SoundEvent::ShieldHit),
         "shield hit sound event", test);

  GameWorld overlays;
  overlays.startNewGame();
  overlays.stepFixed({});
  InputState pause;
  pause.press(Action::Pause);
  overlays.stepFixed(pause);
  expect(overlays.state() == GameState::Paused, "pause state", test);
  overlays.stepFixed(pause);
  expect(overlays.state() == GameState::Running, "pause resume", test);
}

void testFixedStepAndControls()
{
  constexpr const char* test = "fixed step and controls";
  using namespace geocube::core;
  GameWorld world(12);
  world.startNewGame();
  InputState thrust;
  thrust.setHeld(Action::ThrustForward, true);
  world.advance(1.0 / 30.0, thrust);
  const float speedAfterTwoSteps = length(world.player().velocity);
  expect(speedAfterTwoSteps > 0.0f, "fixed step thrust", test);
  expect(speedAfterTwoSteps < kMaxVelocity, "velocity cap", test);
  world.stepFixed(thrust);
  const float speedBeforeStop = length(world.player().velocity);
  const double fuelBeforeStop = world.thrustRemainingSeconds();
  InputState stop;
  stop.press(Action::FullStop);
  world.advance(kFixedStepSeconds, stop);
  const float speedAfterStop = length(world.player().velocity);
  expect(std::fabs(world.thrustRemainingSeconds() -
                   (fuelBeforeStop - kThrustUsageRate * kFixedStepSeconds)) <
             0.0001,
         "full stop uses thrust fuel", test);
  expectNear(speedAfterStop,
             speedBeforeStop - 500.0f * static_cast<float>(kFixedStepSeconds),
             "full stop braking rate", test);
  expect(speedAfterStop > 0.0f, "full stop brakes instead of teleporting",
         test);
  world.stepFixed({});
  expect(lengthSquared(world.player().velocity) == 0.0f,
         "full stop reaches zero", test);

  InputState zoom;
  zoom.setHeld(Action::ZoomIn, true);
  world.stepFixed(zoom);
  expect(world.fieldOfView() < 0.9f, "zoom in", test);

  const Vec3 directionBeforeAim = world.player().direction;
  InputState aim;
  aim.setHeld(Action::AimRight, true);
  world.stepFixed(aim);
  expect(!nearlyEqual(directionBeforeAim, world.player().direction),
         "arrow aim changes ship direction", test);

  expect(kWorldBoundary == 1000.0f, "legacy world boundary", test);
  expectNear(kMaxVelocity, 100.0f * 1000.0f / 60.0f,
             "legacy maximum velocity", test);
}

void testLevelTimerAndShield()
{
  constexpr const char* test = "level timer and shield";
  using namespace geocube::core;
  GameWorld world(15);
  world.startNewGame();
  expect(world.levelElapsedSeconds() == 0.0, "timer starts at zero", test);
  expect(world.shieldRemainingSeconds() == kShieldMaximumSeconds,
         "shield starts full", test);
  world.stepFixed({});
  expect(world.levelElapsedSeconds() == 0.0,
         "loading does not advance timer", test);

  InputState shield;
  shield.setHeld(Action::Shield, true);
  for (int step = 0; step < 180; ++step)
    world.stepFixed(shield);
  expect(world.levelElapsedSeconds() >= 2.99,
         "timer counts during running gameplay", test);
  expect(!world.player().shield, "shield turns off when depleted", test);
  expect(world.shieldRemainingSeconds() <= 0.0001,
         "shield depletes after three seconds", test);

  for (int step = 0; step < 60; ++step)
    world.stepFixed({});
  expect(std::fabs(world.shieldRemainingSeconds() - 0.1) < 0.0001,
         "shield recharges at ten percent", test);

  GameWorld frameWorld(16);
  frameWorld.startNewGame();
  frameWorld.stepFixed({});
  for (int frame = 0; frame < 30; ++frame)
    frameWorld.advance(0.1, shield);
  expect(!frameWorld.player().shield &&
             frameWorld.shieldRemainingSeconds() <= 0.0001,
         "shield uses wall-clock gameplay time", test);

  GameWorld thrustWorld(17);
  thrustWorld.startNewGame();
  thrustWorld.stepFixed({});
  InputState thrust;
  thrust.setHeld(Action::ThrustForward, true);
  for (int step = 0; step < 300; ++step)
    thrustWorld.stepFixed(thrust);
  expect(thrustWorld.thrustRemainingSeconds() <= 0.0001,
         "thrust fuel depletes after five seconds", test);
  for (int step = 0; step < 60; ++step)
    thrustWorld.stepFixed({});
  expect(std::fabs(thrustWorld.thrustRemainingSeconds() - 0.2) < 0.0001,
         "thrust fuel recharges at twenty percent", test);

  world.clearRocks();
  world.stepFixed({});
  expect(world.levelIndex() == 1 && world.levelElapsedSeconds() == 0.0,
         "timer resets for the next level", test);
}

void testHighScores()
{
  constexpr const char* test = "high scores";
  using namespace geocube::core;
  std::vector<HighScoreEntry> scores{{"A", 200, 2}, {"B", 100, 1},
                                     {"C", 50, 1}};
  expect(insertHighScore(scores, {"New", 150, 3}, 3),
         "insert high score", test);
  expect(scores.size() == 3 && scores[0].playerName == "A" &&
             scores[1].playerName == "New" && scores[2].playerName == "B",
         "high score ordering", test);
  expect(!insertHighScore(scores, {"Low", 25, 1}, 3),
         "reject low score", test);
  expect(insertHighScore(scores, {"Top", 500, 5}, 3),
         "insert top score", test);
  expect(scores[0].playerName == "Top" && scores[0].score == 500,
         "top score ordering", test);
}

} // namespace

int main()
{
  testVectorMath();
  testCollisionPrimitives();
  testLevelsAndSplitting();
  testLevelWrapping();
  testStateAndLives();
  testFixedStepAndControls();
  testLevelTimerAndShield();
  testHighScores();

  if (failures != 0) {
    std::cerr << failures << " core test assertion(s) failed\n";
    return EXIT_FAILURE;
  }

  std::cout << "All remastered core tests passed\n";
  return EXIT_SUCCESS;
}
