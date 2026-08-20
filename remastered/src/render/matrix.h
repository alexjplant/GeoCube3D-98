#ifndef GEOCUBE_RENDER_MATRIX_H
#define GEOCUBE_RENDER_MATRIX_H

#include "core/math.h"

#include <cmath>

namespace geocube::render {

struct Mat4 {
  float values[16]{};

  static Mat4 identity()
  {
    Mat4 result;
    result.values[0] = 1.0f;
    result.values[5] = 1.0f;
    result.values[10] = 1.0f;
    result.values[15] = 1.0f;
    return result;
  }

  const float* data() const { return values; }
};

inline Mat4 operator*(const Mat4& left, const Mat4& right)
{
  Mat4 result;
  for (int column = 0; column < 4; ++column) {
    for (int row = 0; row < 4; ++row) {
      result.values[column * 4 + row] =
          left.values[0 * 4 + row] * right.values[column * 4 + 0] +
          left.values[1 * 4 + row] * right.values[column * 4 + 1] +
          left.values[2 * 4 + row] * right.values[column * 4 + 2] +
          left.values[3 * 4 + row] * right.values[column * 4 + 3];
    }
  }
  return result;
}

inline Mat4 translation(const core::Vec3& value)
{
  Mat4 result = Mat4::identity();
  result.values[12] = value.x;
  result.values[13] = value.y;
  result.values[14] = value.z;
  return result;
}

inline Mat4 orientation(const core::Vec3& direction, const core::Vec3& up)
{
  const core::Vec3 forward = core::normalized(direction);
  const core::Vec3 right = core::normalized(core::cross(up, forward));
  const core::Vec3 correctedUp = core::normalized(core::cross(forward, right));
  Mat4 result = Mat4::identity();
  result.values[0] = right.x;
  result.values[1] = right.y;
  result.values[2] = right.z;
  result.values[4] = correctedUp.x;
  result.values[5] = correctedUp.y;
  result.values[6] = correctedUp.z;
  result.values[8] = forward.x;
  result.values[9] = forward.y;
  result.values[10] = forward.z;
  return result;
}

inline Mat4 rotationY(float radians)
{
  Mat4 result = Mat4::identity();
  const float cosine = std::cos(radians);
  const float sine = std::sin(radians);
  result.values[0] = cosine;
  result.values[2] = -sine;
  result.values[8] = sine;
  result.values[10] = cosine;
  return result;
}

inline Mat4 scale(float value)
{
  Mat4 result = Mat4::identity();
  result.values[0] = value;
  result.values[5] = value;
  result.values[10] = value;
  return result;
}

inline Mat4 perspective(float verticalFieldOfView, float aspect,
                        float nearPlane, float farPlane)
{
  const float tangent = std::tan(verticalFieldOfView * 0.5f);
  Mat4 result{};
  result.values[0] = 1.0f / (aspect * tangent);
  result.values[5] = 1.0f / tangent;
  result.values[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
  result.values[11] = -1.0f;
  result.values[14] = -(2.0f * farPlane * nearPlane) /
                     (farPlane - nearPlane);
  return result;
}

inline Mat4 orthographic(float left, float right, float top, float bottom,
                         float nearPlane, float farPlane)
{
  Mat4 result = Mat4::identity();
  result.values[0] = 2.0f / (right - left);
  result.values[5] = 2.0f / (top - bottom);
  result.values[10] = -2.0f / (farPlane - nearPlane);
  result.values[12] = -(right + left) / (right - left);
  result.values[13] = -(top + bottom) / (top - bottom);
  result.values[14] = -(farPlane + nearPlane) / (farPlane - nearPlane);
  return result;
}

inline Mat4 lookAt(const core::Vec3& eye, const core::Vec3& target,
                   const core::Vec3& up)
{
  const core::Vec3 forward = core::normalized(target - eye);
  const core::Vec3 side = core::normalized(core::cross(forward, up));
  const core::Vec3 correctedUp = core::cross(side, forward);
  Mat4 result = Mat4::identity();
  result.values[0] = side.x;
  result.values[4] = side.y;
  result.values[8] = side.z;
  result.values[1] = correctedUp.x;
  result.values[5] = correctedUp.y;
  result.values[9] = correctedUp.z;
  result.values[2] = -forward.x;
  result.values[6] = -forward.y;
  result.values[10] = -forward.z;
  result.values[12] = -core::dot(side, eye);
  result.values[13] = -core::dot(correctedUp, eye);
  result.values[14] = core::dot(forward, eye);
  return result;
}

} // namespace geocube::render

#endif
