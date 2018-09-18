#define _USE_MATH_DEFINES
#include "ren/disk.h"
#include <cmath>
#include "ren/plane.h"
#include "ren/rng.h"

using namespace ren;

Disk::Disk(const Mat4 &local_to_world, Real radius)
    : Shape(local_to_world), radius_(radius) {}

bool Disk::Intersect(const Ray &ray, Real &t, SurfaceDiff &surface_diff) {
  Plane p(local_to_world_);
  if (!p.Intersect(ray, t, surface_diff)) {
    return false;
  }
  Vec3 origin(local_to_world_[3]);
  return Length(origin - surface_diff.p) <= radius_;
}

SurfaceDiff Disk::SamplePoint(Real &pdf) {
  SurfaceDiff surface;
  Real theta = 2 * M_PI * rng::Uniform();
  Real r = radius_ * std::sqrt(rng::Uniform());
  pdf = 1 / Area();
  surface.p = local_to_world_ *
              Vec4(Vec3(r * std::cos(theta), 0, r * std::sin(theta)), 1);
  surface.y = local_to_world_ * Vec4(0, 1, 0, 0);
  surface.x = Normalize(NormalTo(surface.y));
  surface.z = Cross(surface.x, surface.y);
  return surface;
}

Real Disk::Area() const { return M_PI * radius_ * radius_; }

Real Disk::Pdf() const { return 1 / Area(); }
