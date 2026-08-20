#ifndef GEOCUBE_CORE_HULLS_H
#define GEOCUBE_CORE_HULLS_H

#include "core/convex_hull.h"

namespace geocube::core {

const ConvexHull& playerHull();

const ConvexHull& cubeHull();
const ConvexHull& rodHull();
const ConvexHull& sphereHull();
const ConvexHull& coneHull();
const ConvexHull& rockHull();

CollisionGeometry defaultCollisionGeometry();

} // namespace geocube::core

#endif
