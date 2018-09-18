#ifndef REN_TRIANGLE_H_
#define REN_TRIANGLE_H_
#include <vector>
#include "ren/shape.h"
#include "ren/vec.h"
namespace ren {
// Class representing a triangle mesh.
class TriangleMesh : public Shape {
 public:
  TriangleMesh(const Mat4 &local_to_world, const std::vector<Vec3> &vertices,
               const std::vector<int> &indices);
  virtual bool Intersect(const Ray &ray, Real &t,
                         SurfaceDiff &surface_diff) override;
  virtual SurfaceDiff SamplePoint(Real &pdf) override;
  virtual Real Area() const override;

 private:
  bool Intersect(const Ray &ray, int i0, int i1, int i2, Real &t,
                 SurfaceDiff &surface_diff);
  Real Area(int i0, int i1, int i2) const;
  std::vector<Vec3> vertices_;
  std::vector<int> indices_;
  std::vector<Real> probabilities_;
  Real surface_area_;
};

}  // namespace ren
#endif  // REN_TRIANGLE_H_
