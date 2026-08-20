#ifndef GEOCUBE_CORE_CONVEX_HULL_H
#define GEOCUBE_CORE_CONVEX_HULL_H

#include "core/math.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace geocube::core {

struct ConvexHull {
  std::vector<Vec3> localVertices;
  std::vector<std::array<std::size_t, 3>> faces;
  std::vector<std::array<std::size_t, 2>> edges;
};

struct CollisionGeometry {
  ConvexHull player;
  std::array<ConvexHull, 5> rocks;
  ConvexHull bullet;
  float playerScale = 1.0f;
  bool playerRotationY180 = false;
  std::array<float, 5> rockScaleFactors{{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}};
  float bulletScale = 1.0f;
};

struct HullTransform {
  Vec3 position;
  Vec3 xAxis;
  Vec3 yAxis;
  Vec3 zAxis;
};

ConvexHull makeConvexHull(
    const std::vector<Vec3>& vertices,
    const std::vector<std::uint32_t>& indices = {});
CollisionGeometry defaultCollisionGeometry();

// SAT-based intersection test for two transformed convex hulls.
//  aX/aY/aZ are the world-space basis vectors for hull A (columns of the
//  local-to-world transform). They may include non-uniform scale.
bool hullsIntersect(const ConvexHull& a, const Vec3& aPos,
                    const Vec3& aX, const Vec3& aY, const Vec3& aZ,
                    const ConvexHull& b, const Vec3& bPos,
                    const Vec3& bX, const Vec3& bY, const Vec3& bZ);

float hullsDistance(const ConvexHull& a, const HullTransform& aTransform,
                    const ConvexHull& b, const HullTransform& bTransform);

bool sweptHullsIntersect(const ConvexHull& a, const HullTransform& aStart,
                         const HullTransform& aEnd, const ConvexHull& b,
                         const HullTransform& bStart,
                         const HullTransform& bEnd);

} // namespace geocube::core

#endif
