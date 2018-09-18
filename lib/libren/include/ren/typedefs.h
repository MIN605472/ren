#ifndef REN_TYPEDEFS_H_
#define REN_TYPEDEFS_H_
#include "ren/mat.h"
#include "ren/vec.h"
namespace ren {
// Just some convenient typedefs.
typedef double Real;

typedef VecFun<Real, 2> Vec2;
typedef VecFun<int, 2> Vec2i;
typedef VecFun<Real, 3> Vec3;
typedef VecFun<int, 3> Vec3i;
typedef VecFun<Real, 4> Vec4;
typedef VecFun<int, 4> Vec4i;

typedef MatBase<Real, 4> Mat4;
typedef MatBase<int, 4> Mat4i;
typedef MatBase<Real, 3> Mat3;
typedef MatBase<int, 3> Mat3i;

}  // namespace ren
#endif  // REN_TYPEDEFS_H_
