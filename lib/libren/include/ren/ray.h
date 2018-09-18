#ifndef REN_RAY_H_
#define REN_RAY_H_
#include <cmath>
#include <limits>
#include "ren/mat.h"
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {
class Ray {
 public:
  Ray(const Vec3 &origin, const Vec3 &direction,
      Real tmax = std::numeric_limits<Real>::infinity());
  Ray(const Vec3 &direction, Real tmax = std::numeric_limits<Real>::infinity());
  Vec3 GetPoint(Real distance) const;
  Vec3 origin() const;
  Vec3 direction() const;
  Real tmax() const;
  void set_tmax(Real tmax);
  // Transform the ray direcion and position according to the given transform
  // matrix.
  // @param transform the transform matrix
  // @return the new ray transformed
  Ray Transform(const Mat4 &transform) const;

 private:
  Real tmax_;
  Vec3 origin_;
  Vec3 direction_;
};
}  // namespace ren
#endif  // REN_RAY_H_
