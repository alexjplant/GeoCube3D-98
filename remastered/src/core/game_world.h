#ifndef GEOCUBE_CORE_GAME_WORLD_H
#define GEOCUBE_CORE_GAME_WORLD_H

#include "core/math.h"
#include "core/convex_hull.h"
#include "core/collision.h"
#include "core/simulation_scheduler.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace geocube::core {

inline constexpr int kStartingLives = 3;
inline constexpr std::size_t kMaximumBullets = 16;
inline constexpr double kPlayerHitDelaySeconds = 8.0;
inline constexpr double kShieldMaximumSeconds = 3.0;
inline constexpr double kShieldUsageRate = 1.0;
inline constexpr double kShieldRechargeRate = kShieldUsageRate * 0.10;
inline constexpr double kThrustMaximumSeconds = 5.0;
inline constexpr double kThrustUsageRate = 1.0;
inline constexpr double kThrustRechargeRate = kThrustUsageRate * 0.20;
inline constexpr double kFireMaximumSeconds = 2.0;
inline constexpr double kFireUsageRate = 1.0;
inline constexpr double kFireRechargeRate = kFireUsageRate * 0.10;
inline constexpr double kFireDelaySeconds = 1.0;
inline constexpr double kFireShotsPerSecond = 8.0;
inline constexpr double kFireShotUsageSeconds =
    kFireUsageRate / kFireShotsPerSecond;

enum class GameState {
  Loading,
  Running,
  Paused,
  PlayerHit,
  GameOver,
  HighScore,
};

enum class Action : std::uint8_t {
  AimUp,
  AimDown,
  AimLeft,
  AimRight,
  ThrustForward,
  ThrustBackward,
  ThrustLeft,
  ThrustRight,
  Fire,
  Shield,
  Pause,
  FullStop,
  HighScores,
  Quit,
  ZoomIn,
  ZoomOut,
  Confirm,
  Back,
  Count,
};

enum class SoundEvent : std::uint8_t {
  Fire,
  Hit,
  PlayerHit,
  ShieldHit,
  Count,
};

constexpr std::size_t soundEventIndex(SoundEvent event)
{
  return static_cast<std::size_t>(event);
}

constexpr std::size_t actionIndex(Action action)
{
  return static_cast<std::size_t>(action);
}

class InputState final {
public:
  bool isHeld(Action action) const { return m_held[actionIndex(action)]; }
  bool wasPressed(Action action) const
  {
    return m_pressed[actionIndex(action)];
  }

  void setHeld(Action action, bool held)
  {
    m_held[actionIndex(action)] = held;
  }

  void press(Action action)
  {
    m_held[actionIndex(action)] = true;
    m_pressed[actionIndex(action)] = true;
  }

  void release(Action action)
  {
    m_held[actionIndex(action)] = false;
  }

  void clearPressed()
  {
    m_pressed.fill(false);
  }

  void clear(Action action)
  {
    m_held[actionIndex(action)] = false;
    m_pressed[actionIndex(action)] = false;
  }

  void reset()
  {
    m_held.fill(false);
    m_pressed.fill(false);
  }

  bool anyPressed() const
  {
    for (bool pressed : m_pressed) {
      if (pressed)
        return true;
    }
    return false;
  }

private:
  std::array<bool, actionIndex(Action::Count)> m_held{};
  std::array<bool, actionIndex(Action::Count)> m_pressed{};
};

enum class RockSize : int {
  Large = 200,
  Medium = 100,
  Small = 50,
};

enum class RockType : std::uint8_t {
  Cube = 0,
  Rod = 1,
  Sphere = 2,
  Cone = 3,
  Rock = 4,
};

struct LevelDefinition {
  Vec3 background;
  int largeRocks = 0;
  RockType largeType = RockType::Cube;
  int mediumRocks = 0;
  RockType mediumType = RockType::Cube;
  int smallRocks = 0;
  RockType smallType = RockType::Cube;
  const char* music = "";
};

const std::array<LevelDefinition, 5>& levelDefinitions();
int rockSizeUnits(RockSize size);

struct PlayerState {
  Vec3 position;
  Vec3 direction{0.0f, 0.0f, 1.0f};
  Vec3 up{0.0f, 1.0f, 0.0f};
  Vec3 velocity;
  Vec3 thrust;
  bool shield = false;
};

struct Rock {
  std::uint32_t id = 0;
  Vec3 position;
  Vec3 velocity;
  RockSize size = RockSize::Large;
  RockType type = RockType::Cube;
  float radius = 0.0f;
};

enum class BulletState {
  Active,
  Exploding,
};

struct Bullet {
  std::uint32_t id = 0;
  Vec3 position;
  Vec3 velocity;
  float age = 0.0f;
  BulletState state = BulletState::Active;
};

class GameWorld final {
public:
  explicit GameWorld(std::uint32_t randomSeed = 0x47504333u);

  void startNewGame(std::string playerName = {}, int startingLevel = 0);
  void previewLevel(int levelIndex);
  void advance(double elapsedSeconds, const InputState& input);
  void stepFixed(const InputState& input);

  GameState state() const { return m_state; }
  const PlayerState& player() const { return m_player; }
  const std::vector<Rock>& rocks() const { return m_rocks; }
  const std::vector<Bullet>& bullets() const { return m_bullets; }
  const std::string& playerName() const { return m_playerName; }
  const LevelDefinition& currentLevel() const;
  float distanceToRockSurface(const Rock& rock) const;
  void setCollisionGeometry(CollisionGeometry geometry)
  {
    m_collisionGeometry = std::move(geometry);
  }

  int score() const { return m_score; }
  int lives() const { return m_lives; }
  int levelIndex() const { return m_levelIndex; }
  int levelWrap() const { return m_levelWrap; }
  int levelNumber() const { return m_levelIndex + 1 + m_levelWrap * 5; }
  float fieldOfView() const { return m_fieldOfView; }
  bool quitRequested() const { return m_quitRequested; }
  double levelElapsedSeconds() const { return m_levelElapsedSeconds; }
  float cubeBoundary() const { return currentCubeBoundary(); }
  double cubeShrinkWarningRemainingSeconds() const;
  double shieldRemainingSeconds() const { return m_shieldRemainingSeconds; }
  double thrustRemainingSeconds() const { return m_thrustRemainingSeconds; }
  double fireRemainingSeconds() const { return m_fireRemainingSeconds; }
  float velocityMagnitudeRate() const { return m_velocityMagnitudeRate; }
  float thrustMagnitudeRate() const { return m_thrustMagnitudeRate; }
  bool soundEventPending(SoundEvent event) const
  {
    return m_soundEvents[soundEventIndex(event)];
  }
  void clearSoundEvents() { m_soundEvents.fill(false); }
  float shieldChargeFraction() const
  {
    return static_cast<float>(m_shieldRemainingSeconds /
                              kShieldMaximumSeconds);
  }
  float thrustFuelFraction() const
  {
    return static_cast<float>(m_thrustRemainingSeconds /
                              kThrustMaximumSeconds);
  }
  float fireChargeFraction() const
  {
    return static_cast<float>(m_fireRemainingSeconds /
                              kFireMaximumSeconds);
  }
  double playerHitRemainingSeconds() const
  {
    return m_playerHitElapsed < kPlayerHitDelaySeconds
               ? kPlayerHitDelaySeconds - m_playerHitElapsed
               : 0.0;
  }

  // These operations are also the renderer-independent spawn interface used
  // by level setup and deterministic gameplay tests.
  std::uint32_t spawnRock(RockSize size, RockType type, Vec3 position,
                          Vec3 velocity = {});
  std::uint32_t spawnBullet(Vec3 position, Vec3 velocity);
  void clearRocks();
  void clearBullets();
  void setPlayerPosition(Vec3 position) { m_player.position = position; }
  void setPlayerVelocity(Vec3 velocity) { m_player.velocity = velocity; }

private:
  void setupLevel();
  void advanceToNextLevel();
  void updateRunning(const InputState& input);
  void updateCubeShrink();
  void updateShield(const InputState& input, double elapsedSeconds);
  void updateThrustFuel(const InputState& input, double elapsedSeconds);
  void updateFire(const InputState& input, double elapsedSeconds);
  bool fireBullet();
  void emitSoundEvent(SoundEvent event)
  {
    m_soundEvents[soundEventIndex(event)] = true;
  }
  void updatePlayerAim(const InputState& input);
  void updatePlayerThrust(const InputState& input);
  void updateBullets();
  void splitRock(std::size_t rockIndex);
  void handlePlayerCollision(
      const HullTransform& playerStart,
      const std::vector<std::pair<std::uint32_t, HullTransform>>& rockStarts);
  void handlePlayerHit();
  Vec3 randomPosition();
  Vec3 randomVelocity();
  Vec3 randomOffset(float extent);
  void normalizePlayerOrientation();
  const ConvexHull& hullForRock(RockType type) const;
  float rockScale(RockType type, float radius) const;
  void playerHullBasis(Vec3& x, Vec3& y, Vec3& z) const;
  float currentCubeBoundary() const;

  std::mt19937 m_random;
  std::string m_playerName;
  GameState m_state = GameState::Loading;
  PlayerState m_player;
  std::vector<Rock> m_rocks;
  std::vector<Bullet> m_bullets;
  std::uint32_t m_nextEntityId = 1;
  int m_levelIndex = 0;
  int m_levelWrap = 0;
  int m_score = 0;
  int m_lives = kStartingLives;
  float m_fieldOfView = 0.9f;
  SimulationScheduler m_scheduler;
  double m_playerHitElapsed = 0.0;
  double m_levelElapsedSeconds = 0.0;
  double m_shieldRemainingSeconds = kShieldMaximumSeconds;
  double m_thrustRemainingSeconds = kThrustMaximumSeconds;
  double m_fireRemainingSeconds = kFireMaximumSeconds;
  double m_fireHeldSeconds = 0.0;
  double m_fireAutoElapsedSeconds = 0.0;
  float m_velocityMagnitudeRate = 0.0f;
  float m_thrustMagnitudeRate = 0.0f;
  bool m_shieldUpdatedByAdvance = false;
  bool m_thrustFuelUpdatedByAdvance = false;
  bool m_fireUpdatedByAdvance = false;
  bool m_fullStopRequested = false;
  double m_cubeShrinkElapsedSeconds = 0.0;
  double m_cubeShrinkStartBoundary = kWorldBoundary;
  double m_cubeShrinkTargetBoundary = kWorldBoundary;
  std::array<bool, soundEventIndex(SoundEvent::Count)> m_soundEvents{};
  bool m_quitRequested = false;
  CollisionGeometry m_collisionGeometry = defaultCollisionGeometry();
};

} // namespace geocube::core

#endif
