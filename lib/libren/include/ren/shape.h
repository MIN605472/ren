#ifndef REN_SHAPE_H_
#define REN_SHAPE_H_
#include "ren/ray.h"
#include "ren/surface_diff.h"
#include "ren/typedefs.h"

namespace ren {
// Generic shape interface.
class Shape {
 public:
  Shape(const Mat4 &local_to_world);
  // Test for interesection with a given ray.
  // @param ray the ray to test with
  // @param t the distance along the ray where it intersected
  // @param surface_diff the geometric information about the
  // intersction point
  // @return true if the ray \p ray intersected with this shape, false otherwise
  virtual bool Intersect(const Ray &ray, Real &t,
                         SurfaceDiff &surface_diff) = 0;
  // Sampled a point of the shape.
  // @param pdf the probability of sampling the returned point
  // @return the geometric information at the sampled point
  virtual SurfaceDiff SamplePoint(Real &pdf) = 0;
  virtual ~Shape() = default;
  const Mat4 &world_to_local() const;
  const Mat4 &local_to_world() const;
  virtual Real Area() const = 0;

 protected:
  Mat4 world_to_local_;
  Mat4 local_to_world_;
};
}  // namespace ren
#endif  // REN_SHAPE_H_
