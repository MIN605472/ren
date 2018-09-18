#ifndef REN_LIGHT_H_
#define REN_LIGHT_H_
#include "ren/light.h"
#include "ren/mat.h"
#include "ren/surface_diff.h"
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {
// Generic light class.
class Light {
 public:
  Light(const Mat4 &local_to_world, const Vec3 &power);
  // Sample incident radiance at a point.
  // @param surface_scene the object containing information about the
  // differntial surface where incident radiance is to be calculated
  // @param surface_light the object containing information about the
  // differential surface at the sampled point on the light
  // @param pdf the probability of sampling that point
  // @return the incident radiance at \p surface_scene from the sampled point on
  // the light \p surface_light
  virtual Vec3 SampleLi(const SurfaceDiff &surface_scene,
                        SurfaceDiff &surface_light, Real &pdf) = 0;
  // Sample emitted radiance.
  // @param point the sampled point on the light
  // @param dir the sampled directionn
  // @param pdf_point the probability of sampling point \p point
  // @param pdf_dir the probability of sampling the direction \p dir
  // @return the emitted radiance from point \p point to direction \p dir
  virtual Vec3 SampleLe(SurfaceDiff &point, Vec3 &dir, Real &pdf_point,
                        Real &pdf_dir) = 0;
  virtual Vec3 power();

 protected:
  Mat4 local_to_world_;
  Mat4 world_to_local_;
  Vec3 power_;
};
}  // namespace ren
#endif  // REN_LIGHT_H_
