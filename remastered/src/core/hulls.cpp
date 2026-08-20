#include "core/hulls.h"

#include <array>

namespace geocube::core {

namespace {

template <std::size_t NVerts, std::size_t NFaces>
ConvexHull makeHull(const std::array<Vec3, NVerts>& vertices,
                    const std::array<std::array<std::size_t, 3>, NFaces>& faces)
{
  ConvexHull h;
  h.localVertices.assign(vertices.begin(), vertices.end());
  h.faces.reserve(faces.size());
  for (const auto& f : faces)
    h.faces.push_back(f);
  for (const auto& f : faces) {
    auto addEdge = [&h](std::size_t first, std::size_t second) {
      if (first > second)
        std::swap(first, second);
      for (const auto& edge : h.edges)
        if (edge[0] == first && edge[1] == second)
          return;
      h.edges.push_back({first, second});
    };
    addEdge(f[0], f[1]);
    addEdge(f[1], f[2]);
    addEdge(f[2], f[0]);
  }
  return h;
}

} // namespace

// Remastered mesh scaled by 20.0f.
const ConvexHull& playerHull()
{
  static const ConvexHull hull = makeHull(
      std::array<Vec3, 5>{{{0.0f, 0.0f, 30.0f},
                           {-20.0f, -10.0f, -20.0f},
                           {20.0f, -10.0f, -20.0f},
                           {0.0f, 10.0f, -20.0f},
                           {0.0f, 0.0f, -20.0f}}},
      std::array<std::array<std::size_t, 3>, 6>{{{{0, 1, 2}},
                                                  {{0, 2, 3}},
                                                  {{0, 3, 1}},
                                                  {{1, 3, 4}},
                                                  {{3, 2, 4}},
                                                  {{2, 1, 4}}}});
  return hull;
}

const ConvexHull& cubeHull()
{
  static const ConvexHull hull = makeHull(
      std::array<Vec3, 8>{{{-0.5f, -0.5f, -0.5f},
                           {0.5f, -0.5f, -0.5f},
                           {0.5f, 0.5f, -0.5f},
                           {-0.5f, 0.5f, -0.5f},
                           {-0.5f, -0.5f, 0.5f},
                           {0.5f, -0.5f, 0.5f},
                           {0.5f, 0.5f, 0.5f},
                           {-0.5f, 0.5f, 0.5f}}},
      std::array<std::array<std::size_t, 3>, 12>{{{{0, 1, 2}},
                                                   {{2, 3, 0}},
                                                   {{1, 5, 6}},
                                                   {{6, 2, 1}},
                                                   {{5, 4, 7}},
                                                   {{7, 6, 5}},
                                                   {{4, 0, 3}},
                                                   {{3, 7, 4}},
                                                   {{3, 2, 6}},
                                                   {{6, 7, 3}},
                                                   {{4, 5, 1}},
                                                   {{1, 0, 4}}}});
  return hull;
}

const ConvexHull& rodHull()
{
  static const ConvexHull hull = makeHull(
      std::array<Vec3, 8>{{{-0.5f, -1.25f, -0.5f},
                           {0.5f, -1.25f, -0.5f},
                           {0.5f, 1.25f, -0.5f},
                           {-0.5f, 1.25f, -0.5f},
                           {-0.5f, -1.25f, 0.5f},
                           {0.5f, -1.25f, 0.5f},
                           {0.5f, 1.25f, 0.5f},
                           {-0.5f, 1.25f, 0.5f}}},
      std::array<std::array<std::size_t, 3>, 12>{{{{0, 1, 2}},
                                                   {{2, 3, 0}},
                                                   {{1, 5, 6}},
                                                   {{6, 2, 1}},
                                                   {{5, 4, 7}},
                                                   {{7, 6, 5}},
                                                   {{4, 0, 3}},
                                                   {{3, 7, 4}},
                                                   {{3, 2, 6}},
                                                   {{6, 7, 3}},
                                                   {{4, 5, 1}},
                                                   {{1, 0, 4}}}});
  return hull;
}

// Octahedron: 6 vertices, 8 triangular faces.
const ConvexHull& sphereHull()
{
  static const ConvexHull hull = makeHull(
      std::array<Vec3, 6>{{{0.0f, 1.0f, 0.0f},
                           {0.0f, -1.0f, 0.0f},
                           {1.0f, 0.0f, 0.0f},
                           {-1.0f, 0.0f, 0.0f},
                           {0.0f, 0.0f, 1.0f},
                           {0.0f, 0.0f, -1.0f}}},
      std::array<std::array<std::size_t, 3>, 8>{{{{0, 2, 4}},
                                                  {{0, 4, 3}},
                                                  {{0, 3, 5}},
                                                  {{0, 5, 2}},
                                                  {{1, 4, 2}},
                                                  {{1, 3, 4}},
                                                  {{1, 5, 3}},
                                                  {{1, 2, 5}}}});
  return hull;
}

// Tetrahedron approximating a cone.
const ConvexHull& coneHull()
{
  static const ConvexHull hull = makeHull(
      std::array<Vec3, 4>{{{0.0f, 1.0f, 0.0f},
                           {1.0f, -1.0f, 0.0f},
                           {-1.0f, -1.0f, 0.0f},
                           {0.0f, -1.0f, 1.0f}}},
      std::array<std::array<std::size_t, 3>, 4>{{{{0, 1, 2}},
                                                  {{0, 2, 3}},
                                                  {{0, 3, 1}},
                                                  {{1, 3, 2}}}});
  return hull;
}

// Same octahedron as sphereHull.
const ConvexHull& rockHull()
{
  static const ConvexHull hull = makeHull(
      std::array<Vec3, 6>{{{0.0f, 1.0f, 0.0f},
                           {0.0f, -1.0f, 0.0f},
                           {1.0f, 0.0f, 0.0f},
                           {-1.0f, 0.0f, 0.0f},
                           {0.0f, 0.0f, 1.0f},
                           {0.0f, 0.0f, -1.0f}}},
      std::array<std::array<std::size_t, 3>, 8>{{{{0, 2, 4}},
                                                  {{0, 4, 3}},
                                                  {{0, 3, 5}},
                                                  {{0, 5, 2}},
                                                  {{1, 4, 2}},
                                                  {{1, 3, 4}},
                                                  {{1, 5, 3}},
                                                  {{1, 2, 5}}}});
  return hull;
}

CollisionGeometry defaultCollisionGeometry()
{
  CollisionGeometry geometry;
  geometry.player = playerHull();
  geometry.rocks[0] = cubeHull();
  geometry.rocks[1] = rodHull();
  geometry.rocks[2] = sphereHull();
  geometry.rocks[3] = coneHull();
  geometry.rocks[4] = rockHull();
  geometry.bullet = cubeHull();
  geometry.rockScaleFactors[0] = 0.5f;
  geometry.bulletScale = 4.0f;
  return geometry;
}

} // namespace geocube::core
