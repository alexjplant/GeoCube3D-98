#include "core/game_world.h"

#include "core/collision.h"

#include <algorithm>
#include <cmath>

namespace geocube::core {
namespace {

// Legacy gameplay.cpp scales ship.x by .20 and uses the original 1000-unit
// cube with 500 units/s^2 thrust and a 750-unit/s bullet launch speed.
constexpr float kPlayerRadius = 10.0f;
constexpr float kBulletRadius = 1.0f;
constexpr float kThrustAcceleration = 500.0f;
constexpr float kBulletSpeed = 750.0f;
constexpr float kBulletLifetime = 5.0f;
constexpr float kMinimumFieldOfView = 0.10f;
constexpr float kMaximumFieldOfView = 1.0f;
constexpr float kFieldOfViewStep = 0.05f;
constexpr float kRotationSpeedRadians = 50.0f * 0.01745329251994f;

const std::array<LevelDefinition, 5> kLevels{{
    {{0.0f, 0.0f, 0.85f}, 1, RockType::Cube, 0, RockType::Cube, 0,
     RockType::Cube, "music.mid"},
    {{0.0f, 0.85f, 0.0f}, 2, RockType::Rod, 0, RockType::Rod, 0,
     RockType::Rod, "synth.mid"},
    {{0.4f, 0.0f, 0.4f}, 3, RockType::Sphere, 0, RockType::Sphere, 0,
     RockType::Sphere, "reggae.mid"},
    {{0.0f, 0.4f, 0.4f}, 4, RockType::Cone, 0, RockType::Cone, 0,
     RockType::Cone, "spin.mid"},
    {{0.85f, 0.0f, 0.0f}, 5, RockType::Rock, 0, RockType::Rock, 0,
     RockType::Rock, "funk.mid"},
}};

bool hasMovementInput(const InputState& input)
{
  return input.isHeld(Action::ThrustForward) ||
         input.isHeld(Action::ThrustBackward) ||
         input.isHeld(Action::ThrustLeft) ||
         input.isHeld(Action::ThrustRight);
}

} // namespace

const std::array<LevelDefinition, 5>& levelDefinitions()
{
  return kLevels;
}

int rockSizeUnits(RockSize size)
{
  return static_cast<int>(size);
}

GameWorld::GameWorld(std::uint32_t randomSeed) : m_random(randomSeed)
{
  startNewGame();
}

void GameWorld::startNewGame(std::string playerName, int startingLevel)
{
  m_playerName = std::move(playerName);
  m_state = GameState::Loading;
  m_player = {};
  m_rocks.clear();
  m_bullets.clear();
  m_nextEntityId = 1;
  m_levelIndex = std::clamp(startingLevel, 0, 4);
  m_levelWrap = 0;
  m_score = 0;
  m_lives = kStartingLives;
  m_fieldOfView = 0.9f;
  m_accumulator = 0.0;
  m_playerHitElapsed = 0.0;
  m_levelElapsedSeconds = 0.0;
  m_shieldRemainingSeconds = kShieldMaximumSeconds;
  m_thrustRemainingSeconds = kThrustMaximumSeconds;
  m_fullStopRequested = false;
  clearSoundEvents();
  m_quitRequested = false;
}

void GameWorld::previewLevel(int levelIndex)
{
  m_levelIndex = std::clamp(levelIndex, 0, 4);
  m_levelWrap = 0;
  m_state = GameState::Loading;
  m_levelElapsedSeconds = 0.0;
  m_shieldRemainingSeconds = kShieldMaximumSeconds;
  m_thrustRemainingSeconds = kThrustMaximumSeconds;
  m_fullStopRequested = false;
  clearSoundEvents();
  setupLevel();
}

void GameWorld::advance(double elapsedSeconds, const InputState& input)
{
  if (m_quitRequested)
    return;

  const double frameSeconds =
      std::clamp(elapsedSeconds, 0.0, kMaximumFrameSeconds);
  m_accumulator += frameSeconds;
  m_shieldUpdatedByAdvance = m_state == GameState::Running;
  m_thrustFuelUpdatedByAdvance = m_state == GameState::Running;
  // Resource duration follows gameplay time even when fixed-step catch-up is capped.
  const bool canUseResources = !input.wasPressed(Action::Quit) &&
                               !input.wasPressed(Action::Pause) &&
                               !input.wasPressed(Action::HighScores);
  if (m_shieldUpdatedByAdvance && canUseResources)
    updateShield(input, frameSeconds);
  if (m_thrustFuelUpdatedByAdvance && canUseResources)
    updateThrustFuel(input, frameSeconds);

  int steps = 0;
  bool firstStep = true;
  while (m_accumulator >= kFixedStepSeconds &&
         steps < kMaximumCatchUpSteps) {
    InputState stepInput = input;
    if (!firstStep)
      stepInput.clearPressed();
    stepFixed(stepInput);
    m_accumulator -= kFixedStepSeconds;
    ++steps;
    firstStep = false;
  }

  if (steps == kMaximumCatchUpSteps &&
      m_accumulator >= kFixedStepSeconds)
    m_accumulator = 0.0;
  m_shieldUpdatedByAdvance = false;
  m_thrustFuelUpdatedByAdvance = false;
}

void GameWorld::stepFixed(const InputState& input)
{
  switch (m_state) {
  case GameState::Loading:
    setupLevel();
    m_state = GameState::Running;
    return;

  case GameState::Paused:
    if (input.wasPressed(Action::Pause) || input.wasPressed(Action::Confirm))
      m_state = GameState::Running;
    return;

  case GameState::HighScore:
    if (input.anyPressed())
      m_state = GameState::Running;
    return;

  case GameState::PlayerHit:
    handlePlayerHit();
    return;

  case GameState::GameOver:
    return;

  case GameState::Running:
    updateRunning(input);
    return;
  }
}

const LevelDefinition& GameWorld::currentLevel() const
{
  return kLevels[static_cast<std::size_t>(m_levelIndex)];
}

std::uint32_t GameWorld::spawnRock(RockSize size, RockType type,
                                   Vec3 position, Vec3 velocity)
{
  Rock rock;
  rock.id = m_nextEntityId++;
  rock.position = position;
  rock.velocity = velocity;
  rock.size = size;
  rock.type = type;
  rock.radius = static_cast<float>(rockSizeUnits(size));
  m_rocks.push_back(rock);
  return rock.id;
}

std::uint32_t GameWorld::spawnBullet(Vec3 position, Vec3 velocity)
{
  if (m_bullets.size() >= kMaximumBullets)
    return 0;

  Bullet bullet;
  bullet.id = m_nextEntityId++;
  bullet.position = position;
  bullet.velocity = velocity;
  m_bullets.push_back(bullet);
  return bullet.id;
}

void GameWorld::clearRocks()
{
  m_rocks.clear();
}

void GameWorld::clearBullets()
{
  m_bullets.clear();
}

void GameWorld::setupLevel()
{
  m_rocks.clear();
  m_bullets.clear();
  m_player.position = {};
  m_player.velocity = {};
  m_player.shield = false;
  m_fullStopRequested = false;
  m_player.direction = {0.0f, 0.0f, 1.0f};
  m_player.up = {0.0f, 1.0f, 0.0f};

  const LevelDefinition& level = currentLevel();
  const int extraRocks = 2 * m_levelWrap;
  for (int i = 0; i < level.largeRocks + extraRocks; ++i)
    spawnRock(RockSize::Large, level.largeType, randomPosition(),
              randomVelocity());
  for (int i = 0; i < level.mediumRocks + extraRocks; ++i)
    spawnRock(RockSize::Medium, level.mediumType, randomPosition(),
              randomVelocity());
  for (int i = 0; i < level.smallRocks + extraRocks; ++i)
    spawnRock(RockSize::Small, level.smallType, randomPosition(),
              randomVelocity());
}

void GameWorld::advanceToNextLevel()
{
  ++m_levelIndex;
  if (m_levelIndex >= static_cast<int>(kLevels.size())) {
    m_levelIndex = 0;
    ++m_levelWrap;
  }
  m_levelElapsedSeconds = 0.0;
  setupLevel();
}

void GameWorld::updateRunning(const InputState& input)
{
  if (input.wasPressed(Action::Quit)) {
    m_quitRequested = true;
    return;
  }

  if (input.wasPressed(Action::Pause)) {
    m_state = GameState::Paused;
    return;
  }

  if (input.wasPressed(Action::HighScores)) {
    m_state = GameState::HighScore;
    return;
  }

  m_levelElapsedSeconds += kFixedStepSeconds;

  if (input.isHeld(Action::ZoomIn))
    m_fieldOfView = std::max(kMinimumFieldOfView,
                             m_fieldOfView - kFieldOfViewStep);
  if (input.isHeld(Action::ZoomOut))
    m_fieldOfView = std::min(kMaximumFieldOfView,
                             m_fieldOfView + kFieldOfViewStep);

  if (!m_shieldUpdatedByAdvance)
    updateShield(input, kFixedStepSeconds);
  updatePlayerAim(input);

  if (input.wasPressed(Action::FullStop))
    m_fullStopRequested = true;
  if (!m_thrustFuelUpdatedByAdvance)
    updateThrustFuel(input, kFixedStepSeconds);
  updatePlayerThrust(input);

  if (input.wasPressed(Action::Fire)) {
    const Vec3 direction = normalized(m_player.direction);
    spawnBullet(m_player.position,
                direction * kBulletSpeed + m_player.velocity);
  }

  if (advanceAndReflect(m_player.position, m_player.velocity,
                        static_cast<float>(kFixedStepSeconds)))
    emitSoundEvent(SoundEvent::Hit);
  for (Rock& rock : m_rocks)
    advanceAndReflect(rock.position, rock.velocity,
                      static_cast<float>(kFixedStepSeconds));

  updateBullets();
  handlePlayerCollision();

  if (m_state == GameState::Running && m_rocks.empty())
    advanceToNextLevel();
}

void GameWorld::updatePlayerAim(const InputState& input)
{
  Vec3 right = normalized(cross(m_player.up, m_player.direction));
  float yaw = 0.0f;
  float pitch = 0.0f;
  if (input.isHeld(Action::AimLeft))
    yaw += kRotationSpeedRadians * static_cast<float>(kFixedStepSeconds);
  if (input.isHeld(Action::AimRight))
    yaw -= kRotationSpeedRadians * static_cast<float>(kFixedStepSeconds);
  if (input.isHeld(Action::AimUp))
    pitch += kRotationSpeedRadians * static_cast<float>(kFixedStepSeconds);
  if (input.isHeld(Action::AimDown))
    pitch -= kRotationSpeedRadians * static_cast<float>(kFixedStepSeconds);

  if (yaw != 0.0f) {
    m_player.direction =
        rotateAroundAxis(m_player.direction, m_player.up, yaw);
    m_player.up = rotateAroundAxis(m_player.up, m_player.up, yaw);
  }
  if (pitch != 0.0f && lengthSquared(right) > 0.0f) {
    m_player.direction = rotateAroundAxis(m_player.direction, right, pitch);
    m_player.up = rotateAroundAxis(m_player.up, right, pitch);
  }
  normalizePlayerOrientation();
}

void GameWorld::updatePlayerThrust(const InputState& input)
{
  const float thrustStep =
      kThrustAcceleration * static_cast<float>(kFixedStepSeconds);
  if (m_fullStopRequested) {
    const float speed = length(m_player.velocity);
    if (speed <= thrustStep) {
      m_player.velocity = {};
      m_fullStopRequested = false;
    } else if (m_thrustRemainingSeconds <= 0.0) {
      m_fullStopRequested = false;
    } else {
      m_player.velocity = normalized(m_player.velocity) * (speed - thrustStep);
    }
    return;
  }

  if (!hasMovementInput(input))
    return;
  if (m_thrustRemainingSeconds <= 0.0)
    return;

  const Vec3 right = normalized(cross(m_player.up, m_player.direction));
  const Vec3 forward = normalized(m_player.direction);
  const Vec3 up = normalized(m_player.up);
  Vec3 delta;

  if (input.isHeld(Action::ThrustForward))
    delta += forward;
  if (input.isHeld(Action::ThrustBackward))
    delta -= forward;
  if (input.isHeld(Action::ThrustRight))
    delta += right;
  if (input.isHeld(Action::ThrustLeft))
    delta -= right;

  // The legacy thrust model accelerates by 500 world units per second.
  m_player.velocity += delta * thrustStep;
  const float speed = length(m_player.velocity);
  if (speed > kMaxVelocity)
    m_player.velocity = normalized(m_player.velocity) * kMaxVelocity;

  (void)up;
}

void GameWorld::updateShield(const InputState& input, double elapsedSeconds)
{
  if (input.isHeld(Action::Shield) && m_shieldRemainingSeconds > 0.0) {
    const double usage = kShieldUsageRate * elapsedSeconds;
    if (m_shieldRemainingSeconds <= usage + 1.0e-9)
      m_shieldRemainingSeconds = 0.0;
    else
      m_shieldRemainingSeconds -= usage;
    m_player.shield = m_shieldRemainingSeconds > 0.0;
    return;
  }

  m_player.shield = false;
  if (!input.isHeld(Action::Shield))
    m_shieldRemainingSeconds = std::min(
        kShieldMaximumSeconds, m_shieldRemainingSeconds +
                                   kShieldRechargeRate * elapsedSeconds);
}

void GameWorld::updateThrustFuel(const InputState& input,
                                 double elapsedSeconds)
{
  const bool stopping =
      m_fullStopRequested ||
      (input.wasPressed(Action::FullStop) &&
       lengthSquared(m_player.velocity) > 0.0f);
  if (hasMovementInput(input) || stopping) {
    if (m_thrustRemainingSeconds <= 0.0)
      return;

    const double usage = kThrustUsageRate * elapsedSeconds;
    if (m_thrustRemainingSeconds <= usage + 1.0e-9)
      m_thrustRemainingSeconds = 0.0;
    else
      m_thrustRemainingSeconds -= usage;
    return;
  }

  m_thrustRemainingSeconds = std::min(
      kThrustMaximumSeconds,
      m_thrustRemainingSeconds + kThrustRechargeRate * elapsedSeconds);
}

void GameWorld::updateBullets()
{
  for (Bullet& bullet : m_bullets) {
    if (bullet.state == BulletState::Exploding) {
      bullet.age = kBulletLifetime;
      continue;
    }

    const Vec3 oldPosition = bullet.position;
    Vec3 nextPosition = bullet.position;
    Vec3 nextVelocity = bullet.velocity;
    const bool hitBorder = advanceAndReflect(
        nextPosition, nextVelocity, static_cast<float>(kFixedStepSeconds));
    bullet.age += static_cast<float>(kFixedStepSeconds);
    bullet.position = nextPosition;
    bullet.velocity = nextVelocity;

    if (hitBorder) {
      emitSoundEvent(SoundEvent::Hit);
      bullet.state = BulletState::Exploding;
      continue;
    }
    if (bullet.age > kBulletLifetime) {
      bullet.state = BulletState::Exploding;
      continue;
    }

    for (std::size_t rockIndex = 0; rockIndex < m_rocks.size();
         ++rockIndex) {
      const Rock& rock = m_rocks[rockIndex];
      if (segmentIntersectsSphere(oldPosition, bullet.position,
                                   rock.position,
                                   rock.radius + kBulletRadius)) {
        emitSoundEvent(SoundEvent::Hit);
        bullet.state = BulletState::Exploding;
        splitRock(rockIndex);
        break;
      }
    }
  }

  m_bullets.erase(
      std::remove_if(m_bullets.begin(), m_bullets.end(),
                     [](const Bullet& bullet) {
                       return bullet.state == BulletState::Exploding &&
                              bullet.age >= kBulletLifetime;
                     }),
      m_bullets.end());
}

void GameWorld::splitRock(std::size_t rockIndex)
{
  const Rock destroyed = m_rocks[rockIndex];
  m_rocks.erase(m_rocks.begin() + static_cast<std::ptrdiff_t>(rockIndex));

  switch (destroyed.size) {
  case RockSize::Large:
    for (int i = 0; i < 4; ++i)
      spawnRock(RockSize::Medium, currentLevel().mediumType,
                destroyed.position + randomOffset(100.0f), randomVelocity());
    m_score += 50;
    break;

  case RockSize::Medium:
    for (int i = 0; i < 2; ++i)
      spawnRock(RockSize::Small, currentLevel().smallType,
                destroyed.position + randomOffset(50.0f), randomVelocity());
    m_score += 100;
    break;

  case RockSize::Small:
    m_score += 200;
    break;
  }
}

void GameWorld::handlePlayerCollision()
{
  for (const Rock& rock : m_rocks) {
    if (!spheresIntersect(m_player.position, kPlayerRadius, rock.position,
                          rock.radius))
      continue;

    if (m_player.shield) {
      emitSoundEvent(SoundEvent::ShieldHit);
      return;
    }

    emitSoundEvent(SoundEvent::PlayerHit);
    m_state = GameState::PlayerHit;
    m_playerHitElapsed = 0.0;
    m_player.velocity = {};
    clearBullets();
    return;
  }
}

void GameWorld::handlePlayerHit()
{
  m_playerHitElapsed += kFixedStepSeconds;
  if (m_playerHitElapsed + 1.0e-9 < kPlayerHitDelaySeconds)
    return;

  --m_lives;
  m_playerHitElapsed = 0.0;
  if (m_lives <= 0) {
    m_state = GameState::GameOver;
    return;
  }

  setupLevel();
  m_state = GameState::Running;
}

Vec3 GameWorld::randomPosition()
{
  std::uniform_real_distribution<float> distance(200.0f,
                                                  kWorldBoundary - 200.0f);
  std::uniform_int_distribution<int> sign(0, 1);
  auto signedDistance = [&]() {
    const float value = distance(m_random);
    return sign(m_random) == 0 ? -value : value;
  };
  return {signedDistance(), signedDistance(), signedDistance()};
}

Vec3 GameWorld::randomVelocity()
{
  std::uniform_real_distribution<float> component(-150.0f, 150.0f);
  return {component(m_random), component(m_random), component(m_random)};
}

Vec3 GameWorld::randomOffset(float extent)
{
  std::uniform_real_distribution<float> component(-extent / 2.0f,
                                                    extent / 2.0f);
  return {component(m_random), component(m_random), component(m_random)};
}

void GameWorld::normalizePlayerOrientation()
{
  m_player.direction = normalized(m_player.direction);
  Vec3 right = normalized(cross(m_player.up, m_player.direction));
  if (lengthSquared(right) <= 0.0f) {
    m_player.up = {0.0f, 1.0f, 0.0f};
    right = normalized(cross(m_player.up, m_player.direction));
  }
  m_player.up = normalized(cross(m_player.direction, right));
}

} // namespace geocube::core
