#include "core/collision.h"

#include <algorithm>

namespace geocube::core {

bool spheresIntersect(const Vec3& firstPosition, float firstRadius,
                      const Vec3& secondPosition, float secondRadius)
{
  const float radius = firstRadius + secondRadius;
  return lengthSquared(firstPosition - secondPosition) <= radius * radius;
}

bool cylindersIntersect(const CylinderBounds& first,
                        const CylinderBounds& second)
{
  const float firstBottom = first.position.y;
  const float firstTop = first.position.y + first.height;
  const float secondBottom = second.position.y;
  const float secondTop = second.position.y + second.height;

  const bool verticalOverlap = firstBottom <= secondTop &&
                               firstTop >= secondBottom;
  if (!verticalOverlap)
    return false;

  const Vec3 firstXZ{first.position.x, 0.0f, first.position.z};
  const Vec3 secondXZ{second.position.x, 0.0f, second.position.z};
  return spheresIntersect(firstXZ, first.radius, secondXZ, second.radius);
}

bool pointInAabb(const Vec3& point, const Aabb& bounds)
{
  return point.x >= bounds.min.x && point.x <= bounds.max.x &&
         point.y >= bounds.min.y && point.y <= bounds.max.y &&
         point.z >= bounds.min.z && point.z <= bounds.max.z;
}

bool segmentIntersectsSphere(const Vec3& start, const Vec3& end,
                             const Vec3& center, float radius)
{
  const Vec3 segment = end - start;
  const float segmentLengthSquared = lengthSquared(segment);
  float fraction = 0.0f;
  if (segmentLengthSquared > 0.0f) {
    fraction = dot(center - start, segment) / segmentLengthSquared;
    fraction = std::clamp(fraction, 0.0f, 1.0f);
  }

  const Vec3 closest = start + segment * fraction;
  return lengthSquared(closest - center) <= radius * radius;
}

bool advanceAndReflect(Vec3& position, Vec3& velocity, float seconds,
                       float boundary)
{
  position += velocity * seconds;
  bool collided = false;

  auto reflectAxis = [&collided](float& coordinate, float& component,
                                 float limit) {
    if (coordinate > limit) {
      coordinate = limit;
      component = -std::fabs(component);
      collided = true;
    } else if (coordinate < -limit) {
      coordinate = -limit;
      component = std::fabs(component);
      collided = true;
    }
  };

  reflectAxis(position.x, velocity.x, boundary);
  reflectAxis(position.y, velocity.y, boundary);
  reflectAxis(position.z, velocity.z, boundary);
  return collided;
}

} // namespace geocube::core
