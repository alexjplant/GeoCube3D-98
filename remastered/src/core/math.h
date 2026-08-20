#ifndef GEOCUBE_CORE_MATH_H
#define GEOCUBE_CORE_MATH_H

#include <cmath>

namespace geocube::core {

struct Vec3 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  constexpr Vec3() = default;
  constexpr Vec3(float xValue, float yValue, float zValue)
      : x(xValue), y(yValue), z(zValue)
  {
  }

  constexpr Vec3 operator+(const Vec3& other) const
  {
    return {x + other.x, y + other.y, z + other.z};
  }

  constexpr Vec3 operator-(const Vec3& other) const
  {
    return {x - other.x, y - other.y, z - other.z};
  }

  constexpr Vec3 operator-() const
  {
    return {-x, -y, -z};
  }

  constexpr Vec3 operator*(float scalar) const
  {
    return {x * scalar, y * scalar, z * scalar};
  }

  constexpr Vec3 operator/(float scalar) const
  {
    return {x / scalar, y / scalar, z / scalar};
  }

  constexpr bool operator==(const Vec3& other) const
  {
    return x == other.x && y == other.y && z == other.z;
  }

  constexpr bool operator!=(const Vec3& other) const
  {
    return !(*this == other);
  }

  constexpr Vec3& operator+=(const Vec3& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  constexpr Vec3& operator-=(const Vec3& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  constexpr Vec3& operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  constexpr Vec3& operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }
};

constexpr Vec3 operator*(float scalar, const Vec3& value)
{
  return value * scalar;
}

constexpr float dot(const Vec3& left, const Vec3& right)
{
  return left.x * right.x + left.y * right.y + left.z * right.z;
}

constexpr Vec3 cross(const Vec3& left, const Vec3& right)
{
  return {left.y * right.z - left.z * right.y,
          left.z * right.x - left.x * right.z,
          left.x * right.y - left.y * right.x};
}

constexpr float lengthSquared(const Vec3& value)
{
  return dot(value, value);
}

inline float length(const Vec3& value)
{
  return std::sqrt(lengthSquared(value));
}

inline Vec3 normalized(const Vec3& value)
{
  const float magnitude = length(value);
  if (magnitude <= 0.0f)
    return {};
  return value / magnitude;
}

inline Vec3 reflect(const Vec3& value, const Vec3& normal)
{
  const Vec3 unitNormal = normalized(normal);
  if (lengthSquared(unitNormal) <= 0.0f)
    return value;
  return value - unitNormal * (2.0f * dot(value, unitNormal));
}

inline Vec3 rotateAroundAxis(const Vec3& value, const Vec3& axis,
                             float radians)
{
  const Vec3 unitAxis = normalized(axis);
  if (lengthSquared(unitAxis) <= 0.0f)
    return value;

  const float cosine = std::cos(radians);
  const float sine = std::sin(radians);
  return value * cosine + cross(unitAxis, value) * sine +
         unitAxis * (dot(unitAxis, value) * (1.0f - cosine));
}

inline bool nearlyEqual(const Vec3& left, const Vec3& right,
                        float epsilon = 0.0001f)
{
  return std::fabs(left.x - right.x) <= epsilon &&
         std::fabs(left.y - right.y) <= epsilon &&
         std::fabs(left.z - right.z) <= epsilon;
}

} // namespace geocube::core

#endif
