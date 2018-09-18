#ifndef REN_PATHTRACER_H_
#define REN_PATHTRACER_H_
#include "ren/pinhole_camera.h"
#include "ren/renderer.h"
#include "ren/scene.h"
namespace ren {
// A Path tracing renderer.
class PathTracer : public Renderer {
 public:
  PathTracer(Scene *scene, PinholeCamera *camera, int spp);
  virtual void Render() override;

 private:
  void RenderRange(int min_y, int max_y);
  Vec3 Li(int i, int j);
  Scene *scene_;
  PinholeCamera *camera_;
  int spp_;
};
}  // namespace ren
#endif  // REN_PATHTRACER_H_
