#include "core/convex_hull.h"
#include "core/hulls.h"
#include "render/matrix.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {

bool near(float actual, float expected)
{
  return std::fabs(actual - expected) < 0.0001f;
}

void expect(bool condition, const char* message)
{
  if (!condition) {
    std::cerr << message << '\n';
    std::exit(EXIT_FAILURE);
  }
}

} // namespace

int main()
{
  using namespace geocube::core;
  const std::vector<Vec3> tetrahedron{{0.0f, 0.0f, 0.0f},
                                      {1.0f, 0.0f, 0.0f},
                                      {0.0f, 1.0f, 0.0f},
                                      {0.0f, 0.0f, 1.0f}};
  const ConvexHull tetra = makeConvexHull(tetrahedron, {3, 2, 1});
  expect(tetra.localVertices.size() == tetrahedron.size(),
         "hull construction uses the point cloud contract");
  expect(!tetra.faces.empty() && !tetra.edges.empty(),
         "tetrahedral hull has topology");
  expect(makeConvexHull({}).localVertices.empty(), "empty hull stays empty");

  const ConvexHull& cube = cubeHull();
  const HullTransform first{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
                            {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
  const HullTransform end{{0.0f, 0.0f, 3.0f}, {1.0f, 0.0f, 0.0f},
                          {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
  const HullTransform stationary{{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f},
                                 {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
  expect(hullsIntersect(cube, first.position, first.xAxis, first.yAxis,
                        first.zAxis, cube, stationary.position,
                        stationary.xAxis, stationary.yAxis,
                        stationary.zAxis),
         "touching hulls intersect");
  expect(near(hullsDistance(cube, first, cube, stationary), 0.0f),
         "contact distance is zero");
  expect(sweptHullsIntersect(cube, first, end, cube, stationary, stationary),
         "swept hull detects an in-between collision");

  using namespace geocube::render;
  const Mat4 identity = Mat4::identity();
  expect(near((identity * translation({1.0f, 2.0f, 3.0f})).values[12], 1.0f),
         "matrix multiplication preserves translation");
  expect(near(scale(2.0f).values[0], 2.0f), "matrix scale");
  expect(near(translation({1.0f, 2.0f, 3.0f}).values[14], 3.0f),
         "matrix translation");
  expect(near(lookAt({0.0f, 0.0f, 1.0f}, {}, {0.0f, 1.0f, 0.0f}).values[14],
              -1.0f),
         "lookAt camera translation");

  std::cout << "Geometry and matrix tests passed\n";
}
