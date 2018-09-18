#ifndef REN_TRANSFORM_H_
#define REN_TRANSFORM_H_
#include "ren/mat.h"

namespace ren {

// Creates a transform matrix generraly used to create the coordinate system of
// a camera.
// @param from the position of the "camera"
// @param to the position where the "camera" is looking
// @param up the up vector or orientation of the "camera"
// @return the transform matrix
template <typename T>
MatBase<T, 4> LookAt(const VecFun<T, 3>& from, const VecFun<T, 3>& to,
                     const VecFun<T, 3>& up) {
  auto look = Normalize(from - to);
  auto right = Cross(Normalize(up), look);
  MatBase<T, 4> camera_to_world;
  camera_to_world[0][0] = right[0];
  camera_to_world[0][1] = right[1];
  camera_to_world[0][2] = right[2];
  camera_to_world[1][0] = up[0];
  camera_to_world[1][1] = up[1];
  camera_to_world[1][2] = up[2];
  camera_to_world[2][0] = look[0];
  camera_to_world[2][1] = look[1];
  camera_to_world[2][2] = look[2];
  camera_to_world[3][0] = from[0];
  camera_to_world[3][1] = from[1];
  camera_to_world[3][2] = from[2];
  camera_to_world[3][3] = 1;
  return camera_to_world;
}

// Creates a translation matrix.
// @param m the matrix to wich it should be applied
// @param t the translation vector
// @return the newly created translation matrix
template <typename T>
MatBase<T, 4> Translate(MatBase<T, 4> m, const VecFun<T, 3>& t) {
  m[3][0] += t[0];
  m[3][1] += t[1];
  m[3][2] += t[2];
  return m;
}

// Creates a rotation matrix.
// @param m the matrix
// @param rad the angle of rotation in radians. Positive rotation is
// counterclockwise given that it is a randed coordinate system.
// @param axis the axis of rotation
// @return the rotation matrix
template <typename T>
MatBase<T, 4> Rotate(const MatBase<T, 4>& m, T rad, const VecFun<T, 3>& axis) {
  MatBase<T, 4> r;
  r[0][0] = std::cos(rad) + axis[0] * axis[0] * (1 - std::cos(rad));
  r[0][1] = axis[1] * axis[0] * (1 - std::cos(rad)) + axis[2] * std::sin(rad);
  r[0][2] = axis[2] * axis[0] * (1 - std::cos(rad)) - axis[1] * std::sin(rad);

  r[1][0] = axis[0] * axis[1] * (1 - std::cos(rad)) - axis[2] * std::sin(rad);
  r[1][1] = std::cos(rad) + axis[1] * axis[1] * (1 - std::cos(rad));
  r[1][2] = axis[2] * axis[1] * (1 - std::cos(rad)) + axis[0] * std::sin(rad);

  r[2][0] = axis[0] * axis[2] * (1 - std::cos(rad)) + axis[1] * std::sin(rad);
  r[2][1] = axis[1] * axis[2] * (1 - std::cos(rad)) - axis[0] * std::sin(rad);
  r[2][2] = std::cos(rad) + axis[2] * axis[2] * (1 - std::cos(rad));
  return m * r;
}

// Creates a transform matrix given the components of a coordinate system.
// @param m the matrix
// @param x the x axis
// @param y the y axis
// @param z the z axis
// @return the matrix
template <typename T>
MatBase<T, 4> Basis(const MatBase<T, 4>& m, const VecFun<T, 3>& x,
                    const VecFun<T, 3>& y, const VecFun<T, 3>& z) {
  MatBase<T, 4> tmp;
  tmp[0][0] = x[0];
  tmp[0][1] = x[1];
  tmp[0][2] = x[2];

  tmp[1][0] = y[0];
  tmp[1][1] = y[1];
  tmp[1][2] = y[2];

  tmp[2][0] = z[0];
  tmp[2][1] = z[1];
  tmp[2][2] = z[2];

  return m * tmp;
}

}  // namespace ren
#endif  // REN_TRANSFORM_H_
