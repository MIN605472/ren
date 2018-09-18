#ifndef REN_AREALIGHT_H_
#define REN_AREALIGHT_H_
#include <memory>
#include "ren/light.h"
#include "ren/shape.h"
#include "ren/typedefs.h"
namespace ren {
// Class that represents diffuse or Lambertian area lights.
class AreaLight : public Light {
 public:
  // Create an area light.
  // @param local_to_world the transform that indicates its position and
  // rotation
  // @param color the emitted radiance
  // @param shape the shape associated with the area light
  AreaLight(const Mat4 &local_to_world, const Vec3 &color,
            std::unique_ptr<Shape> shape);
  virtual Vec3 SampleLi(const SurfaceDiff &surface_scene,
                        SurfaceDiff &surface_light, Real &pdf) override;
  virtual Vec3 SampleLe(SurfaceDiff &point, Vec3 &dir, Real &pdf_point,
                        Real &pdf_dir) override;
  virtual Vec3 L(const SurfaceDiff &surface_scene,
                 const SurfaceDiff &surface_light) const;

 private:
  std::unique_ptr<Shape> shape_;
};
}  // namespace ren
#endif  // REN_AREALIGHT_H_
