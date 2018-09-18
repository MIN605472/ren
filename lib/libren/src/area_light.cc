#define _USE_MATH_DEFINES
#include "ren/area_light.h"
#include "ren/sampling.h"
#include "ren/transform.h"

using namespace ren;

AreaLight::AreaLight(const Mat4 &local_to_world, const Vec3 &color,
                     std::unique_ptr<Shape> shape)
    : Light(local_to_world, color), shape_(std::move(shape)) {}

Vec3 AreaLight::SampleLi(const SurfaceDiff &surface_scene,
                         SurfaceDiff &surface_light, Real &pdf) {
  surface_light = shape_->SamplePoint(pdf);
  auto wo = surface_scene.p - surface_light.p;
  auto dot = Dot(surface_light.y, Normalize(wo));
  pdf *= Length2(wo) / std::abs(dot);
  return dot > 0 ? power_ : Vec3();
}

Vec3 AreaLight::SampleLe(SurfaceDiff &point, Vec3 &dir, Real &pdf_point,
                         Real &pdf_dir) {
  auto sampled_surface_diff = shape_->SamplePoint(pdf_point);
  point = sampled_surface_diff;
  // sampling::UniformDirHemisphere(dir, pdf_dir);
  sampling::CosWeightedDirHemisphere(dir, pdf_dir);
  dir = sampled_surface_diff.x * dir.x + sampled_surface_diff.y * dir.y +
        sampled_surface_diff.z * dir.z;
  return power_;
}

Vec3 AreaLight::L(const SurfaceDiff &surface_scene,
                  const SurfaceDiff &surface_light) const {
  auto v = surface_scene.p - surface_light.p;
  auto dot = Dot(surface_light.y, v);
  return dot > 0 ? power_ : Vec3();
}
