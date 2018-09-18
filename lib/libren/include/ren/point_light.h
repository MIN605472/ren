#ifndef REN_POINTLIGHT_H_
#define REN_POINTLIGHT_H_
#include "ren/light.h"
#include "ren/mat.h"
#include "ren/surface_diff.h"
#include "ren/vec.h"
namespace ren {
class PointLight : public Light {
 public:
  // Constructs a point light.
  // @param local_to_world the transform matrix describing the light's position
  // @param color the emitted radiant intensity
  PointLight(const Mat4 &local_to_world, const Vec3 &color);
  virtual Vec3 SampleLi(const SurfaceDiff &surface_scene,
                        SurfaceDiff &surface_light, Real &pdf) override;
  virtual Vec3 SampleLe(SurfaceDiff &point, Vec3 &dir, Real &pdf_point,
                        Real &pdf_dir) override;
};
}  // namespace ren
#endif  // REN_POINTLIGHT_H_
