#define _USE_MATH_DEFINES
#include "ren/surface_diff.h"
#include <cmath>
#include "ren/object.h"
#include "ren/transform.h"

using namespace ren;
//
// void SurfaceDiff::SetTransforms(const Vec3 &x, const Vec3 &y, const Vec3
// &z,
//                                const Vec3 &point) {
//  world_to_local = Basis(Translate(Mat4(), point), x, y, z);
//  local_to_world = Inverse(world_to_local);
//}