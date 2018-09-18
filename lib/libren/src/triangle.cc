#include "ren/triangle.h"
#include "ren/rng.h"

using namespace ren;

TriangleMesh::TriangleMesh(const Mat4 &local_to_world,
                           const std::vector<Vec3> &vertices,
                           const std::vector<int> &indices)
    : Shape(local_to_world), vertices_(vertices), indices_(indices) {
  surface_area_ = 0;
  for (int i = 0; i < indices_.size(); i += 3) {
    surface_area_ += Area(indices_[i], indices_[i + 1], indices_[i + 2]);
  }
  for (int i = 0; i < indices_.size(); i += 3) {
    probabilities_.push_back(
        Area(indices_[i], indices_[i + 1], indices_[i + 2]) / surface_area_);
  }
}

bool TriangleMesh::Intersect(const Ray &ray, Real &t,
                             SurfaceDiff &surface_diff) {
  t = std::numeric_limits<Real>::max();
  int triangle = -1;
  for (int i = 0; i < indices_.size(); i += 3) {
    Real t_tmp;
    SurfaceDiff surface_tmp;
    if (Intersect(ray, indices_[i], indices_[i + 1], indices_[i + 2], t_tmp,
                  surface_tmp) &&
        t_tmp < t) {
      surface_diff = surface_tmp;
      t = t_tmp;
      triangle = i;
    }
  }
  return triangle > -1;
}

SurfaceDiff TriangleMesh::SamplePoint(Real &pdf) {
  Real cdf = 0;
  auto xi0 = rng::Uniform();
  int triangle_index = -1;
  for (int i = 0; i < probabilities_.size(); ++i) {
    cdf += probabilities_[i];
    if (cdf >= xi0) {
      triangle_index = i * 3;
      break;
    }
  }

  auto xi1 = rng::Uniform();
  auto xi2 = rng::Uniform();
  auto p =
      (1.0 - std::sqrt(xi1)) * vertices_[indices_[triangle_index]] +
      std::sqrt(xi1) * (1.0 - xi2) * vertices_[indices_[triangle_index + 1]] +
      xi2 * std::sqrt(xi1) * vertices_[indices_[triangle_index + 2]];
  SurfaceDiff surface_diff;
  auto e1 = vertices_[indices_[triangle_index + 1]] -
            vertices_[indices_[triangle_index]];
  auto e2 = vertices_[indices_[triangle_index + 2]] -
            vertices_[indices_[triangle_index]];
  surface_diff.p = p;
  surface_diff.x = Normalize(e1);
  surface_diff.y = Normalize(Cross(surface_diff.x, Normalize(e2)));
  surface_diff.z = Cross(surface_diff.x, surface_diff.y);
  pdf = 1.0 / surface_area_;
  return surface_diff;
}

Real TriangleMesh::Area() const { return surface_area_; }

bool TriangleMesh::Intersect(const Ray &ray, int i0, int i1, int i2, Real &t,
                             SurfaceDiff &surface_diff) {
  auto e1 = vertices_[i1] - vertices_[i0];
  auto e2 = vertices_[i2] - vertices_[i0];
  auto q = Cross(ray.direction(), e2);
  auto a = Dot(e1, q);
  if (a > -1E-4 && a < 1E-4) {
    return false;
  }
  auto f = 1.0 / a;
  auto s = ray.origin() - vertices_[i0];
  auto u = f * Dot(s, q);
  if (u < 0.0) {
    return false;
  }
  auto r = Cross(s, e1);
  auto v = f * Dot(ray.direction(), r);
  if (v < 0.0 || u + v > 1.0) {
    return false;
  }
  t = f * Dot(e2, r);
  if (t > 0 && t < ray.tmax()) {
    surface_diff.x = Normalize(e1);
    surface_diff.y = Normalize(Cross(surface_diff.x, Normalize(e2)));
    surface_diff.z = Cross(surface_diff.x, surface_diff.y);
    surface_diff.p = ray.GetPoint(t);
    return true;
  }
  return false;
}

Real TriangleMesh::Area(int i0, int i1, int i2) const {
  auto e1 = vertices_[i1] - vertices_[i0];
  auto e2 = vertices_[i2] - vertices_[i0];
  return Length(Cross(e1, e2)) * 0.5;
}
