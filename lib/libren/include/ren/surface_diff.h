#ifndef REN_SURFACEDIFF_H_
#define REN_SURFACEDIFF_H_
#include "ren/mat.h"
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {
class Object;
// Struct that holds geometric and shading information about the differential
// surface at a point of intersection.
struct SurfaceDiff {
  Vec3 p;  // the point of intersection
  Vec3 x;  // the x axis
  Vec3 y;  // the y axis
  Vec3 z;  // the z axis. x and z form a plane and y is the normal to this plane
  const Object *o;  // the intersected object
};
}  // namespace ren
#endif  // REN_SURFACEDIFF_H_
