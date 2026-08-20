#ifndef GEOCUBE_CORE_COLLISION_H
#define GEOCUBE_CORE_COLLISION_H

#include "core/math.h"

namespace geocube::core {

inline constexpr float kWorldBoundary = 1000.0f;
inline constexpr float kMaxVelocity = 100.0f * 1000.0f / 60.0f;

struct Aabb {
  Vec3 min;
  Vec3 max;
};

struct CylinderBounds {
  Vec3 position;
  float height = 0.0f;
  float radius = 0.0f;
};

bool spheresIntersect(const Vec3& firstPosition, float firstRadius,
                      const Vec3& secondPosition, float secondRadius);
bool cylindersIntersect(const CylinderBounds& first,
                        const CylinderBounds& second);
bool pointInAabb(const Vec3& point, const Aabb& bounds);
bool segmentIntersectsSphere(const Vec3& start, const Vec3& end,
                             const Vec3& center, float radius);

// Advances a bounded entity and reflects it from the six legacy cube walls.
bool advanceAndReflect(Vec3& position, Vec3& velocity, float seconds,
                       float boundary = kWorldBoundary);

} // namespace geocube::core

#endif
