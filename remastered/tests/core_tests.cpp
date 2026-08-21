#include "core/collision.h"
#include "core/convex_hull.h"
#include "core/game_world.h"
#include "core/high_scores.h"
#include "core/hulls.h"
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
   expect(world.spareShips() == 3, "starting spare ships", test);
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
   expect(world.spareShips() == 2, "spare ship decrement", test);
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
   expect(world.spareShips() == 0, "game over spare ship count", test);
   expect(world.state() == GameState::GameOver, "game over state", test);

   GameWorld blockedRespawn(11);
   blockedRespawn.startNewGame();
   blockedRespawn.stepFixed({});
   blockedRespawn.clearRocks();
   blockedRespawn.setPlayerPosition({});
   blockedRespawn.spawnRock(RockSize::Small, RockType::Cube, {});
   blockedRespawn.stepFixed({});
   for (int step = 0; step < 480; ++step)
     blockedRespawn.stepFixed({});
   expect(blockedRespawn.state() == GameState::PlayerHit &&
              blockedRespawn.respawnWaitingForClearance(),
          "respawn waits for center clearance", test);
   const int blockedLevel = blockedRespawn.levelNumber();
   blockedRespawn.clearRocks();
   blockedRespawn.stepFixed({});
   expect(blockedRespawn.state() == GameState::Running &&
              blockedRespawn.levelNumber() == blockedLevel &&
              blockedRespawn.rocks().empty(),
          "clearance respawn preserves level state", test);

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

   const Vec3 upBeforeRoll = world.player().up;
   InputState roll;
   roll.setHeld(Action::RollLeft, true);
   world.stepFixed(roll);
   expect(!nearlyEqual(upBeforeRoll, world.player().up),
          "barrel roll changes ship up vector", test);

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
   expect(std::fabs(thrustWorld.thrustRemainingSeconds() - 0.4) < 0.0001,
          "thrust fuel recharges at forty percent", test);

  world.clearRocks();
  world.stepFixed({});
   expect(world.levelIndex() == 1 && world.levelElapsedSeconds() == 0.0 &&
              world.shieldRemainingSeconds() == kShieldMaximumSeconds &&
              world.thrustRemainingSeconds() == kThrustMaximumSeconds &&
              world.fireRemainingSeconds() == kFireMaximumSeconds,
          "level transition resets timer and resources", test);
}

void testFireControl()
{
  constexpr const char* test = "fire control";
  using namespace geocube::core;
  GameWorld world(44);
  world.startNewGame();
  world.stepFixed({});
  world.clearRocks();
  world.spawnRock(RockSize::Small, RockType::Cube, {0.0f, 500.0f, 0.0f});

  InputState fire;
  fire.press(Action::Fire);
  world.stepFixed(fire);
  expect(world.bullets().size() == 1, "fire press launches one bullet", test);
  expect(world.soundEventPending(SoundEvent::Fire),
         "fire press sound event", test);
  world.clearSoundEvents();
  fire.clearPressed();

  for (int step = 0; step < 58; ++step)
    world.stepFixed(fire);
  expect(world.bullets().size() == 1,
         "held fire waits one second before autofire", test);
  world.stepFixed(fire);
  expect(world.bullets().size() == 2,
         "held fire starts autofire after one second", test);
  expect(std::fabs(world.fireRemainingSeconds() - 1.75) < 0.0001,
         "fire gauge spends one eighth second per shot", test);

  fire.release(Action::Fire);
  for (int step = 0; step < 60; ++step)
    world.stepFixed(fire);
  expect(std::fabs(world.fireRemainingSeconds() - 1.85) < 0.0001,
         "fire gauge recharges at ten percent", test);

  world.clearBullets();
  world.clearSoundEvents();
  world.spawnBullet({990.0f, 0.0f, 0.0f}, {750.0f, 0.0f, 0.0f});
  world.stepFixed({});
  expect(!world.soundEventPending(SoundEvent::Hit),
         "projectile world boundary has no hit sound", test);
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

void testConvexHullDirect()
{
  constexpr const char* test = "convex hull direct";
  using namespace geocube::core;

  // Two unit cubes at the same position should intersect.
  {
    const ConvexHull& cube = cubeHull();
    bool hit = hullsIntersect(cube, {0.0f, 0.0f, 0.0f},
                              {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                              {0.0f, 0.0f, 1.0f}, cube,
                              {0.0f, 0.0f, 0.0f},
                              {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                              {0.0f, 0.0f, 1.0f});
    expect(hit, "identical cubes intersect", test);
  }

  // Player hull inside cube hull.
  {
    const ConvexHull& p = playerHull();
    const ConvexHull& c = cubeHull();
    bool hit = hullsIntersect(p, {0.0f, 0.0f, 0.0f},
                              {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                              {0.0f, 0.0f, 1.0f}, c,
                              {0.0f, 0.0f, 0.0f},
                              {100.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f},
                              {0.0f, 0.0f, 100.0f});
    expect(hit, "player inside large cube intersects", test);
  }

  // Surface distance is zero for contact and positive for separation.
  {
    const ConvexHull& cube = cubeHull();
    const Vec3 identityX{1.0f, 0.0f, 0.0f};
    const Vec3 identityY{0.0f, 1.0f, 0.0f};
    const Vec3 identityZ{0.0f, 0.0f, 1.0f};
    const HullTransform first{{0.0f, 0.0f, 0.0f}, identityX, identityY,
                              identityZ};
    const HullTransform separated{{0.0f, 0.0f, 2.0f}, identityX, identityY,
                                  identityZ};
    const HullTransform touching{{0.0f, 0.0f, 1.0f}, identityX, identityY,
                                 identityZ};
    expectNear(hullsDistance(cube, first, cube, separated), 1.0f,
               "separated cube surface distance", test);
    expectNear(hullsDistance(cube, first, cube, touching), 0.0f,
               "touching cube surface distance", test);
  }
}

void testConvexHullCollision()
{
  constexpr const char* test = "convex hull collision";
  using namespace geocube::core;

  auto toRunning = [](GameWorld& world) {
    // startNewGame leaves state as Loading; one empty step sets it Running.
    world.stepFixed({});
    world.clearRocks();
  };

  // Player-vs-cube: far apart.
  {
    GameWorld world(0);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Cube, {1000.0f, 0.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::Running,
           "player far from cube: no collision", test);
  }

  // Player-vs-cube: overlapping.
  {
    GameWorld world(1);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Cube, {0.0f, 0.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player inside cube: collision", test);
  }

  // Player-vs-cube: touching at edge.
  {
    GameWorld world(2);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    // Cube large radius is 200, hull scale = 100. Local cube is -0.5..0.5,
    // so world half-extent is 50.  Cube at z=80 places its near face at
    // z=30, exactly where the player nose sits.
    world.spawnRock(RockSize::Large, RockType::Cube, {0.0f, 0.0f, 80.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player nose touching cube face: collision", test);
  }

  // Player-vs-cube: just separated.
  {
    GameWorld world(3);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    // Move cube 1 unit farther than the touching test.
    world.spawnRock(RockSize::Large, RockType::Cube, {0.0f, 0.0f, 81.0f});
    world.stepFixed({});
    expect(world.state() == GameState::Running,
           "player nose just outside cube: no collision", test);
  }

  // Player-vs-rod: overlapping (rod is stretched in Y).
  {
    GameWorld world(4);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Rod, {0.0f, 0.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player inside rod: collision", test);
  }

  // Player-vs-rod: touching along long axis.
  {
    GameWorld world(5);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    // Rod large radius=200, scale=200. Local Y is -1.25..1.25, so world Y
    // is -250..250. Place player at Y=250 + ship extent ~10 => 260.
    world.spawnRock(RockSize::Large, RockType::Rod, {0.0f, 260.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player touching rod top: collision", test);
  }

  // Player-vs-sphere: overlapping.
  {
    GameWorld world(6);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Sphere, {0.0f, 0.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player inside sphere: collision", test);
  }

  // Player-vs-cone: overlapping.
  {
    GameWorld world(7);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Cone, {0.0f, 0.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player inside cone: collision", test);
  }

  // Player-vs-rock: overlapping.
  {
    GameWorld world(8);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Rock, {0.0f, 0.0f, 0.0f});
    world.stepFixed({});
    expect(world.state() == GameState::PlayerHit,
           "player inside rock: collision", test);
  }

  // Shield blocks hull collision without player hit.
  {
    GameWorld world(9);
    world.startNewGame();
    toRunning(world);
    world.setPlayerPosition({0.0f, 0.0f, 0.0f});
    world.spawnRock(RockSize::Large, RockType::Cube, {0.0f, 0.0f, 0.0f});
    InputState shield;
    shield.press(Action::Shield);
    world.stepFixed(shield);
    expect(world.state() == GameState::Running,
           "shielded player inside cube: no hit", test);
    expect(world.soundEventPending(SoundEvent::ShieldHit),
           "shielded collision emits shield hit sound", test);
  }

  // Direct hull rotation test: identical cubes, one rotated 90 degrees.
  {
    const ConvexHull& cube = cubeHull();
    // Cube A at origin with identity orientation.
    // Cube B at z=1.0 (just touching face), rotated 90° around Y.
    const Vec3 bPos = {0.0f, 0.0f, 1.0f};
    const Vec3 bX = {0.0f, 0.0f, -1.0f}; // local X maps to -world Z
    const Vec3 bY = {0.0f, 1.0f, 0.0f};  // local Y maps to world Y
    const Vec3 bZ = {1.0f, 0.0f, 0.0f};  // local Z maps to world X
    bool hit = hullsIntersect(cube, {0.0f, 0.0f, 0.0f},
                              {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                              {0.0f, 0.0f, 1.0f}, cube, bPos,
                              bX, bY, bZ);
    expect(hit, "rotated touching cubes intersect", test);
  }
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
  testFireControl();
  testHighScores();
  testConvexHullDirect();
  testConvexHullCollision();

  if (failures != 0) {
    std::cerr << failures << " core test assertion(s) failed\n";
    return EXIT_FAILURE;
  }

  std::cout << "All remastered core tests passed\n";
  return EXIT_SUCCESS;
}
