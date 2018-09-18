#include "ren/ray.h"
#include <cmath>

using namespace ren;

Ray::Ray(const Vec3 &origin, const Vec3 &direction, Real tmax)
    : origin_(origin), direction_(direction), tmax_(tmax) {}

Ray::Ray(const Vec3 &direction, Real tmax)
    : direction_(direction), tmax_(tmax) {}

Vec3 Ray::GetPoint(Real distance) const {
  return origin_ + distance * direction_;
}

Vec3 Ray::origin() const { return origin_; }

Vec3 Ray::direction() const { return direction_; }

Real Ray::tmax() const { return tmax_; }

void Ray::set_tmax(Real tmax) { tmax_ = tmax; }

Ray Ray::Transform(const Mat4 &transform) const {
  return Ray(transform * Vec4(origin_, 1), transform * Vec4(direction_, 0));
}
