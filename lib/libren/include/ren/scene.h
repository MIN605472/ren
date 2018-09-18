#ifndef REN_SCENE_H_
#define REN_SCENE_H_
#include <memory>
#include <vector>
#include "ren/light.h"
#include "ren/object.h"
#include "ren/ray.h"
namespace ren {
// Class that holds information about a scene, i.e., the objects and lights
// forming it.
class Scene {
 public:
  // Test if the ray intersects an object in the scene.
  // @param ray the ray to test with
  // @param surface_diff the geometric and material information about the
  // intersection point
  // @return true if hte ray intersect an object in the scene, false otherwise
  bool Intersect(const Ray &ray, SurfaceDiff &surface_diff) const;
  const std::vector<std::unique_ptr<Light>> &lights() const;
  void AddObject(std::unique_ptr<Object> o);
  void AddLight(std::unique_ptr<Light> l);
  bool AnyObjectWithBsdf(Bsdf::Type type) const;

 private:
  std::vector<std::unique_ptr<Object>> objects_;
  std::vector<std::unique_ptr<Light>> lights_;
};
}  // namespace ren
#endif  // REN_SCENE_H_
