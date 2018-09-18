#ifndef REN_SPHERE_H_
#define REN_SPHERE_H_
#include "ren/shape.h"
#include "ren/vec.h"
namespace ren {
// Class representing a sphere.
class Sphere : public Shape {
 public:
  Sphere(const Mat4 &local_to_world, Real radius);
  Vec3 origin() const;
  Real radius() const;
  virtual bool Intersect(const Ray &ray, Real &t,
                         SurfaceDiff &surface_diff) override;
  virtual SurfaceDiff SamplePoint(Real &pdf) override;
  virtual Real Area() const override;

 private:
  const static Vec3 kOrigin;
  Vec3 origin_;
  Real radius_;
};
}  // namespace ren
#endif  // REN_SPHERE_H_
