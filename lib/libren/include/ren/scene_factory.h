#ifndef REN_SCENEFACTORY_H_
#define REN_SCENEFACTORY_H_
#include <map>
#include <memory>
#include <string>
#include "ren/scene.h"
namespace ren {
// Singleton holding a couple of hardcoded example scenes.
class SceneFactory {
 public:
  Scene *GetScene(const std::string &name);
  static SceneFactory &GetInstance();

 private:
  Scene Cbox();
  Scene CboxBlocks();
  Scene CboxSpheres();
  Scene CboxSphereInside();
  Scene CboxBlocksDisk();
  SceneFactory();
  static std::unique_ptr<SceneFactory> instance_;
  std::map<std::string, Scene> scenes_;
};
}  // namespace ren
#endif  // REN_SCENEFACTORY_H_
