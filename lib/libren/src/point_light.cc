#define _USE_MATH_DEFINES
#include "ren/point_light.h"
#include "ren/ray.h"
#include "ren/sampling.h"

using namespace ren;

PointLight::PointLight(const Mat4 &local_to_world, const Vec3 &color)
    : Light(local_to_world, color) {}

Vec3 PointLight::SampleLi(const SurfaceDiff &surface_scene,
                          SurfaceDiff &surface_light, Real &pdf) {
  pdf = 1;
  surface_light.p = Vec3(local_to_world_[3]);
  auto length2 = Length2(surface_scene.p - surface_light.p);
  return power_ / (length2);
}

Vec3 PointLight::SampleLe(SurfaceDiff &point, Vec3 &dir, Real &pdf_point,
                          Real &pdf_dir) {
  pdf_point = 1;
  point.p = local_to_world_[3];
  sampling::UniformDirSphere(dir, pdf_dir);
  point.y = dir;
  return power_;
}
