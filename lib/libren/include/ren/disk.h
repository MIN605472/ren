#ifndef REN_DISK_H_
#define REN_DISK_H_
#include "ren/shape.h"
#include "ren/typedefs.h"
namespace ren {
// Class that represents a disk or circle.
class Disk : public Shape {
 public:
  Disk(const Mat4 &local_to_world, Real radius);
  virtual bool Intersect(const Ray &ray, Real &t,
                         SurfaceDiff &surface_diff) override;
  virtual SurfaceDiff SamplePoint(Real &pdf) override;
  virtual Real Area() const override;
  Real Pdf() const;

 private:
  Real radius_;
};
}  // namespace ren
#endif  // REN_DISK_H_
