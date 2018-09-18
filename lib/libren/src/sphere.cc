#include "ren/sphere.h"
#include <algorithm>
#include "ren/transform.h"

using namespace ren;

const Vec3 Sphere::kOrigin(0, 0, 0);

Sphere::Sphere(const Mat4& local_to_world, Real radius)
    : Shape(local_to_world),
      origin_(local_to_world * Vec4(kOrigin, 1)),
      radius_(radius) {}

Vec3 Sphere::origin() const { return origin_; }

Real Sphere::radius() const { return radius_; }

bool Sphere::Intersect(const Ray& ray, Real& t, SurfaceDiff& surface_diff) {
  auto tmpV = ray.origin() - origin_;
  Real a = Dot(ray.direction(), ray.direction());
  Real b = Dot(2.0 * ray.direction(), tmpV);
  Real c = Dot(tmpV, tmpV) - pow(radius_, 2.0);
  Real d = pow(b, 2.0) - 4.0 * a * c;
  if (d < 0.0) {
    return false;
  }
  Real t0 = (-b - std::sqrt(d)) / (2.0 * a);
  Real t1 = (-b + std::sqrt(d)) / (2.0 * a);
  if (t0 > t1) {
    std::swap(t0, t1);
  }
  if (t0 < 0) {
    t0 = t1;
    if (t0 < 0) {
      return false;
    }
  }
  t = t0;
  if (t > ray.tmax()) {
    return false;
  }
  auto point = ray.GetPoint(t);
  auto normal = Normalize(point - origin_);
  auto x = Normalize(NormalTo(normal));
  auto z = Cross(normal, x);
  surface_diff.p = point;
  surface_diff.x = x;
  surface_diff.y = normal;
  surface_diff.z = z;
  return true;
}

SurfaceDiff Sphere::SamplePoint(Real& pdf) { return SurfaceDiff(); }

Real Sphere::Area() const { return 0.0; }
