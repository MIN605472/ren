#ifndef REN_RENDERER_H_
#define REN_RENDERER_H_
#include "ren/scene.h"
#include "ren/surface_diff.h"
#include "ren/vec.h"
namespace ren {
// Generic renderer interface.
class Renderer {
 public:
  // Render the final image.
  virtual void Render() = 0;
  // Estimate the direct radiance.
  // @param scene the scene
  // @param surface the point where direct illumination is to be calculated
  // @param wo the outgoing direction
  // @param samples the number of samples per light should be used
  // @return the total estimated radiance due to direct illumiation
  Vec3 EstimateDirectRadiance(const Scene &scene, const SurfaceDiff &surface,
                              const Vec3 &wo, int samples = 1);
};
}  // namespace ren
#endif  // REN_RENDERER_H_
