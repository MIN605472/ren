#ifndef REN_PLANE_H_
#define REN_PLANE_H_
#include "ren/shape.h"
#include "ren/vec.h"
namespace ren {
// Class represeting a plane.
class Plane : public Shape {
 public:
  Plane(const Mat4 &local_to_world);
  Vec3 point() const;
  Vec3 normal() const;
  virtual bool Intersect(const Ray &ray, Real &t,
                         SurfaceDiff &surface_diff) override;
  virtual SurfaceDiff SamplePoint(Real &pdf) override;
  virtual Real Area() const override;

 private:
  const static Vec3 kPoint;
  const static Vec3 kNormal;
  Vec3 point_;
  Vec3 x_;
  Vec3 y_or_normal_;
  Vec3 z_;
};
}  // namespace ren
#endif  // REN_PLANE_H_
