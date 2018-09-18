#include "ren/plane.h"
#include "ren/vec.h"

using namespace ren;

const Vec3 Plane::kPoint(0, 0, 0);

const Vec3 Plane::kNormal(0, 1, 0);

Plane::Plane(const Mat4& local_to_world)
    : Shape(local_to_world),
      point_(local_to_world * Vec4(kPoint, 1)),
      y_or_normal_(local_to_world * Vec4(kNormal, 0)),
      x_(local_to_world * Vec4(1, 0, 0, 0)),
      z_(local_to_world * Vec4(0, 0, 1, 0)) {}

Vec3 Plane::point() const { return point_; }

Vec3 Plane::normal() const { return y_or_normal_; }

bool Plane::Intersect(const Ray& ray, Real& t, SurfaceDiff& surface_diff) {
  Real den = Dot(y_or_normal_, ray.direction());
  if (std::abs(den) > 0.0001) {
    t = Dot(point_ - ray.origin(), y_or_normal_) / den;
    if (t >= 0 && t < ray.tmax()) {
      surface_diff.p = ray.GetPoint(t);
      surface_diff.x = x_;
      surface_diff.y = y_or_normal_;
      surface_diff.z = z_;
      return true;
    }
    return false;
  }
  return false;
}

SurfaceDiff Plane::SamplePoint(Real& pdf) { return SurfaceDiff(); }

Real ren::Plane::Area() const { return 0.0f; }
