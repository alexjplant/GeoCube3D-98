#include "core/convex_hull.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>
#include <limits>

namespace geocube::core {

namespace {

struct Basis {
  Vec3 x;
  Vec3 y;
  Vec3 z;
};

Vec3 transformVertex(const Vec3& local, const Vec3& pos, const Basis& b)
{
  return pos + b.x * local.x + b.y * local.y + b.z * local.z;
}

// Project each transformed vertex onto axis and return [min, max].
void projectOntoAxis(const ConvexHull& hull, const Vec3& pos, const Basis& b,
                     const Vec3& axis, float& outMin, float& outMax)
{
  outMin = 1.0e30f;
  outMax = -1.0e30f;
  for (const Vec3& local : hull.localVertices) {
    const Vec3 world = transformVertex(local, pos, b);
    const float d = dot(world, axis);
    outMin = std::min(outMin, d);
    outMax = std::max(outMax, d);
  }
}

bool intervalOverlap(float minA, float maxA, float minB, float maxB)
{
  return maxA >= minB && maxB >= minA;
}

bool tryAxis(const ConvexHull& a, const Vec3& aPos, const Basis& aBasis,
             const ConvexHull& b, const Vec3& bPos, const Basis& bBasis,
             const Vec3& axis)
{
  if (lengthSquared(axis) <= 1.0e-12f)
    return true; // degenerate axis, skip
  const Vec3 normal = normalized(axis);
  float minA = 0.0f, maxA = 0.0f, minB = 0.0f, maxB = 0.0f;
  projectOntoAxis(a, aPos, aBasis, normal, minA, maxA);
  projectOntoAxis(b, bPos, bBasis, normal, minB, maxB);
  return intervalOverlap(minA, maxA, minB, maxB);
}

std::vector<Vec3> worldVertices(const ConvexHull& hull,
                                const HullTransform& transform)
{
  std::vector<Vec3> result;
  result.reserve(hull.localVertices.size());
  const Basis basis{transform.xAxis, transform.yAxis, transform.zAxis};
  for (const Vec3& vertex : hull.localVertices)
    result.push_back(transformVertex(vertex, transform.position, basis));
  return result;
}

float pointTriangleDistanceSquared(const Vec3& point, const Vec3& a,
                                   const Vec3& b, const Vec3& c)
{
  const Vec3 ab = b - a;
  const Vec3 ac = c - a;
  const Vec3 ap = point - a;
  const float d1 = dot(ab, ap);
  const float d2 = dot(ac, ap);
  if (d1 <= 0.0f && d2 <= 0.0f)
    return lengthSquared(ap);

  const Vec3 bp = point - b;
  const float d3 = dot(ab, bp);
  const float d4 = dot(ac, bp);
  if (d3 >= 0.0f && d4 <= d3)
    return lengthSquared(bp);

  const float vc = d1 * d4 - d3 * d2;
  if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
    const float t = d1 / (d1 - d3);
    return lengthSquared(point - (a + ab * t));
  }

  const Vec3 cp = point - c;
  const float d5 = dot(ab, cp);
  const float d6 = dot(ac, cp);
  if (d6 >= 0.0f && d5 <= d6)
    return lengthSquared(cp);

  const float vb = d5 * d2 - d1 * d6;
  if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
    const float t = d2 / (d2 - d6);
    return lengthSquared(point - (a + ac * t));
  }

  const float va = d3 * d6 - d5 * d4;
  if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
    const Vec3 bc = c - b;
    const float t = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    return lengthSquared(point - (b + bc * t));
  }

  const Vec3 normal = normalized(cross(ab, ac));
  const float distance = dot(point - a, normal);
  return distance * distance;
}

float segmentDistanceSquared(const Vec3& startA, const Vec3& endA,
                             const Vec3& startB, const Vec3& endB)
{
  const Vec3 directionA = endA - startA;
  const Vec3 directionB = endB - startB;
  const Vec3 between = startA - startB;
  const float a = dot(directionA, directionA);
  const float e = dot(directionB, directionB);
  const float f = dot(directionB, between);
  constexpr float epsilon = 1.0e-10f;
  float s = 0.0f;
  float t = 0.0f;

  if (a <= epsilon && e <= epsilon)
    return lengthSquared(startA - startB);
  if (a <= epsilon) {
    t = std::clamp(f / e, 0.0f, 1.0f);
  } else {
    const float c = dot(directionA, between);
    if (e <= epsilon) {
      s = std::clamp(-c / a, 0.0f, 1.0f);
    } else {
      const float b = dot(directionA, directionB);
      const float denominator = a * e - b * b;
      if (denominator != 0.0f)
        s = std::clamp((b * f - c * e) / denominator, 0.0f, 1.0f);
      t = (b * s + f) / e;
      if (t < 0.0f) {
        t = 0.0f;
        s = std::clamp(-c / a, 0.0f, 1.0f);
      } else if (t > 1.0f) {
        t = 1.0f;
        s = std::clamp((b - c) / a, 0.0f, 1.0f);
      }
    }
  }

  return lengthSquared((startA + directionA * s) -
                       (startB + directionB * t));
}

} // namespace

namespace {

void addHullEdge(ConvexHull& hull, std::size_t first, std::size_t second)
{
  if (first > second)
    std::swap(first, second);
  for (const auto& edge : hull.edges) {
    if (edge[0] == first && edge[1] == second)
      return;
  }
  hull.edges.push_back({first, second});
}

ConvexHull makeAabbHull(const std::vector<Vec3>& vertices)
{
  ConvexHull hull;
  if (vertices.empty())
    return hull;
  Vec3 minimum = vertices.front();
  Vec3 maximum = vertices.front();
  for (const Vec3& vertex : vertices) {
    minimum.x = std::min(minimum.x, vertex.x);
    minimum.y = std::min(minimum.y, vertex.y);
    minimum.z = std::min(minimum.z, vertex.z);
    maximum.x = std::max(maximum.x, vertex.x);
    maximum.y = std::max(maximum.y, vertex.y);
    maximum.z = std::max(maximum.z, vertex.z);
  }
  hull.localVertices = {{minimum.x, minimum.y, minimum.z},
                        {maximum.x, minimum.y, minimum.z},
                        {maximum.x, maximum.y, minimum.z},
                        {minimum.x, maximum.y, minimum.z},
                        {minimum.x, minimum.y, maximum.z},
                        {maximum.x, minimum.y, maximum.z},
                        {maximum.x, maximum.y, maximum.z},
                        {minimum.x, maximum.y, maximum.z}};
  hull.faces = {{{0, 1, 2}, {2, 3, 0}, {1, 5, 6}, {6, 2, 1},
                {5, 4, 7}, {7, 6, 5}, {4, 0, 3}, {3, 7, 4},
                {3, 2, 6}, {6, 7, 3}, {4, 5, 1}, {1, 0, 4}}};
  for (const auto& face : hull.faces) {
    addHullEdge(hull, face[0], face[1]);
    addHullEdge(hull, face[1], face[2]);
    addHullEdge(hull, face[2], face[0]);
  }
  return hull;
}

struct HullFace {
  std::size_t a = 0;
  std::size_t b = 0;
  std::size_t c = 0;
  bool removed = false;
};

void orientFace(std::vector<HullFace>& faces, std::size_t a, std::size_t b,
                std::size_t c, std::size_t opposite,
                const std::vector<Vec3>& points)
{
  const Vec3 normal = cross(points[b] - points[a], points[c] - points[a]);
  if (dot(normal, points[opposite] - points[a]) > 0.0f)
    std::swap(b, c);
  faces.push_back({a, b, c, false});
}

void rebuildEdges(ConvexHull& hull)
{
  for (const auto& face : hull.faces) {
    auto addEdge = [&hull](std::size_t first, std::size_t second) {
      if (first > second)
        std::swap(first, second);
      for (const auto& edge : hull.edges)
        if (edge[0] == first && edge[1] == second)
          return;
      hull.edges.push_back({first, second});
    };
    addEdge(face[0], face[1]);
    addEdge(face[1], face[2]);
    addEdge(face[2], face[0]);
  }
}

ConvexHull makeIncrementalHull(const std::vector<Vec3>& input)
{
  std::vector<Vec3> points;
  points.reserve(input.size());
  for (const Vec3& point : input) {
    bool duplicate = false;
    for (const Vec3& existing : points) {
      if (lengthSquared(point - existing) <= 1.0e-10f) {
        duplicate = true;
        break;
      }
    }
    if (!duplicate)
      points.push_back(point);
  }

  if (points.size() < 4)
    return makeAabbHull(points);

  std::size_t first = 0;
  std::size_t second = 0;
  for (std::size_t i = 1; i < points.size(); ++i) {
    if (points[i].x < points[first].x)
      first = i;
    if (points[i].x > points[second].x)
      second = i;
  }
  const Vec3 line = points[second] - points[first];
  if (lengthSquared(line) <= 1.0e-10f)
    return makeAabbHull(points);

  std::size_t third = first;
  float lineDistance = 0.0f;
  for (std::size_t i = 0; i < points.size(); ++i) {
    const float distance = lengthSquared(cross(points[i] - points[first], line));
    if (distance > lineDistance) {
      lineDistance = distance;
      third = i;
    }
  }
  if (third == first || lineDistance <= 1.0e-10f)
    return makeAabbHull(points);

  const Vec3 planeNormal =
      cross(points[second] - points[first], points[third] - points[first]);
  std::size_t fourth = first;
  float planeDistance = 0.0f;
  for (std::size_t i = 0; i < points.size(); ++i) {
    const float distance =
        std::fabs(dot(planeNormal, points[i] - points[first]));
    if (distance > planeDistance) {
      planeDistance = distance;
      fourth = i;
    }
  }
  if (fourth == first || planeDistance <= 1.0e-10f)
    return makeAabbHull(points);

  std::vector<HullFace> faces;
  faces.reserve(points.size() * 4);
  orientFace(faces, first, second, third, fourth, points);
  orientFace(faces, first, fourth, second, third, points);
  orientFace(faces, first, third, fourth, second, points);
  orientFace(faces, second, fourth, third, first, points);

  const Vec3 inside =
      (points[first] + points[second] + points[third] + points[fourth]) /
      4.0f;
  for (std::size_t pointIndex = 0; pointIndex < points.size(); ++pointIndex) {
    if (pointIndex == first || pointIndex == second || pointIndex == third ||
        pointIndex == fourth)
      continue;

    std::vector<std::pair<std::size_t, std::size_t>> horizon;
    bool visibleAny = false;
    for (HullFace& face : faces) {
      if (face.removed)
        continue;
      const Vec3 normal = cross(points[face.b] - points[face.a],
                                points[face.c] - points[face.a]);
      if (dot(normal, points[pointIndex] - points[face.a]) <= 1.0e-5f)
        continue;
      visibleAny = true;
      face.removed = true;
      const std::array<std::pair<std::size_t, std::size_t>, 3> edges{{
          {face.a, face.b}, {face.b, face.c}, {face.c, face.a}}};
      for (const auto& edge : edges) {
        const auto reverse = std::make_pair(edge.second, edge.first);
        const auto existing =
            std::find(horizon.begin(), horizon.end(), reverse);
        if (existing != horizon.end())
          horizon.erase(existing);
        else
          horizon.push_back(edge);
      }
    }
    if (!visibleAny)
      continue;
    for (const auto& edge : horizon) {
      std::size_t a = edge.first;
      std::size_t b = edge.second;
      const Vec3 normal = cross(points[b] - points[a],
                                points[pointIndex] - points[a]);
      if (dot(normal, inside - points[a]) > 0.0f)
        std::swap(a, b);
      faces.push_back({a, b, pointIndex, false});
    }
  }

  ConvexHull hull;
  hull.localVertices = std::move(points);
  for (const HullFace& face : faces) {
    if (!face.removed)
      hull.faces.push_back({face.a, face.b, face.c});
  }
  if (hull.faces.empty())
    return makeAabbHull(input);
  rebuildEdges(hull);
  return hull;
}

} // namespace

ConvexHull makeConvexHull(const std::vector<Vec3>& vertices,
                          const std::vector<std::uint32_t>& indices)
{
  if (vertices.empty())
    return {};
  (void)indices;
  return makeIncrementalHull(vertices);
}

bool hullsIntersect(const ConvexHull& a, const Vec3& aPos,
                    const Vec3& aX, const Vec3& aY, const Vec3& aZ,
                    const ConvexHull& b, const Vec3& bPos,
                    const Vec3& bX, const Vec3& bY, const Vec3& bZ)
{
  if (a.localVertices.empty() || b.localVertices.empty() || a.faces.empty() ||
      b.faces.empty())
    return false;

  const Basis aBasis{aX, aY, aZ};
  const Basis bBasis{bX, bY, bZ};

  // Face normals of A.
  for (const auto& face : a.faces) {
    if (face[0] >= a.localVertices.size() || face[1] >= a.localVertices.size() ||
        face[2] >= a.localVertices.size())
      continue;
    const Vec3 v0 = transformVertex(a.localVertices[face[0]], aPos, aBasis);
    const Vec3 v1 = transformVertex(a.localVertices[face[1]], aPos, aBasis);
    const Vec3 v2 = transformVertex(a.localVertices[face[2]], aPos, aBasis);
    const Vec3 normal = cross(v1 - v0, v2 - v0);
    if (!tryAxis(a, aPos, aBasis, b, bPos, bBasis, normal))
      return false;
  }

  // Face normals of B.
  for (const auto& face : b.faces) {
    if (face[0] >= b.localVertices.size() || face[1] >= b.localVertices.size() ||
        face[2] >= b.localVertices.size())
      continue;
    const Vec3 v0 = transformVertex(b.localVertices[face[0]], bPos, bBasis);
    const Vec3 v1 = transformVertex(b.localVertices[face[1]], bPos, bBasis);
    const Vec3 v2 = transformVertex(b.localVertices[face[2]], bPos, bBasis);
    const Vec3 normal = cross(v1 - v0, v2 - v0);
    if (!tryAxis(a, aPos, aBasis, b, bPos, bBasis, normal))
      return false;
  }

  // Edge cross products (Minkowski difference edges).
  // Collect unique edges from faces.
  auto collectEdgeDirections = [](const ConvexHull& hull, const Basis& b) {
    std::vector<Vec3> dirs;
    for (const auto& edgeIndices : hull.edges) {
      const Vec3 edge =
          b.x * (hull.localVertices[edgeIndices[1]].x -
                 hull.localVertices[edgeIndices[0]].x) +
          b.y * (hull.localVertices[edgeIndices[1]].y -
                 hull.localVertices[edgeIndices[0]].y) +
          b.z * (hull.localVertices[edgeIndices[1]].z -
                 hull.localVertices[edgeIndices[0]].z);
      if (lengthSquared(edge) > 1.0e-12f)
        dirs.push_back(edge);
    }
    return dirs;
  };

  const std::vector<Vec3> aEdges = collectEdgeDirections(a, aBasis);
  const std::vector<Vec3> bEdges = collectEdgeDirections(b, bBasis);

  for (const Vec3& ae : aEdges) {
    for (const Vec3& be : bEdges) {
      const Vec3 axis = cross(ae, be);
      if (!tryAxis(a, aPos, aBasis, b, bPos, bBasis, axis))
        return false;
    }
  }

  return true;
}

float hullsDistance(const ConvexHull& a, const HullTransform& aTransform,
                    const ConvexHull& b, const HullTransform& bTransform)
{
  if (hullsIntersect(a, aTransform.position, aTransform.xAxis,
                     aTransform.yAxis, aTransform.zAxis, b,
                     bTransform.position, bTransform.xAxis,
                     bTransform.yAxis, bTransform.zAxis))
    return 0.0f;

  const std::vector<Vec3> verticesA = worldVertices(a, aTransform);
  const std::vector<Vec3> verticesB = worldVertices(b, bTransform);
  float closestSquared = std::numeric_limits<float>::max();

  for (const Vec3& point : verticesA) {
    for (const auto& face : b.faces) {
      closestSquared = std::min(
          closestSquared,
          pointTriangleDistanceSquared(point, verticesB[face[0]],
                                       verticesB[face[1]], verticesB[face[2]]));
    }
  }
  for (const Vec3& point : verticesB) {
    for (const auto& face : a.faces) {
      closestSquared = std::min(
          closestSquared,
          pointTriangleDistanceSquared(point, verticesA[face[0]],
                                       verticesA[face[1]], verticesA[face[2]]));
    }
  }

  for (const auto& edgeA : a.edges) {
    const Vec3 aStart = verticesA[edgeA[0]];
    const Vec3 aEnd = verticesA[edgeA[1]];
    for (const auto& edgeB : b.edges) {
      closestSquared = std::min(
          closestSquared,
          segmentDistanceSquared(aStart, aEnd, verticesB[edgeB[0]],
                                 verticesB[edgeB[1]]));
    }
  }
  return std::sqrt(std::max(0.0f, closestSquared));
}

bool sweptHullsIntersect(const ConvexHull& a, const HullTransform& aStart,
                         const HullTransform& aEnd, const ConvexHull& b,
                         const HullTransform& bStart,
                         const HullTransform& bEnd)
{
  const float travel = length(aEnd.position - aStart.position) +
                       length(bEnd.position - bStart.position);
  const int samples = std::clamp(static_cast<int>(std::ceil(travel / 2.0f)),
                                 1, 64);
  for (int i = 0; i <= samples; ++i) {
    const float t = static_cast<float>(i) / samples;
    const auto interpolateAxis = [t](const Vec3& start, const Vec3& end) {
      const Vec3 value = start + (end - start) * t;
      const float scale = (length(start) + length(end)) * 0.5f;
      return normalized(value) * scale;
    };
    const HullTransform aTransform{
        aStart.position + (aEnd.position - aStart.position) * t,
        interpolateAxis(aStart.xAxis, aEnd.xAxis),
        interpolateAxis(aStart.yAxis, aEnd.yAxis),
        interpolateAxis(aStart.zAxis, aEnd.zAxis)};
    const HullTransform bTransform{
        bStart.position + (bEnd.position - bStart.position) * t,
        interpolateAxis(bStart.xAxis, bEnd.xAxis),
        interpolateAxis(bStart.yAxis, bEnd.yAxis),
        interpolateAxis(bStart.zAxis, bEnd.zAxis)};
    if (hullsIntersect(a, aTransform.position, aTransform.xAxis,
                       aTransform.yAxis, aTransform.zAxis, b,
                       bTransform.position, bTransform.xAxis,
                       bTransform.yAxis, bTransform.zAxis))
      return true;
  }
  return false;
}

} // namespace geocube::core
