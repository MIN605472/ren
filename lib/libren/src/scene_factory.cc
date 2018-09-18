#define _USE_MATH_DEFINES
#include "ren/scene_factory.h"
#include "ren/area_light.h"
#include "ren/disk.h"
#include "ren/plane.h"
#include "ren/point_light.h"
#include "ren/sphere.h"
#include "ren/transform.h"
#include "ren/triangle.h"

using namespace ren;

std::unique_ptr<SceneFactory> SceneFactory::instance_ = nullptr;

SceneFactory::SceneFactory() : scenes_() {
  scenes_.insert(std::make_pair("cbox_blocks", CboxBlocks()));
  scenes_.insert(std::make_pair("cbox_spheres", CboxSpheres()));
  scenes_.insert(std::make_pair("cbox_sphere_inside", CboxSphereInside()));
  scenes_.insert(std::make_pair("cbox_blocks_disk", CboxBlocksDisk()));
}

Scene *SceneFactory::GetScene(const std::string &name) {
  if (scenes_.find(name) == scenes_.end()) {
    return nullptr;
  }
  return &scenes_[name];
}

SceneFactory &SceneFactory::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = std::unique_ptr<SceneFactory>(new SceneFactory());
  }
  return *instance_;
}

Scene SceneFactory::Cbox() {
  Scene scene;
  std::vector<Vec3> vertices;
  std::vector<int> indices;
  // floor
  // clang-format off
  vertices = {
   Vec3(552.8, 0.0, 0.0),
   Vec3(0.0, 0.0, 0.0),
   Vec3(0.0, 0.0, 559.2),
   Vec3(549.6, 0.0, 559.2)
  };
  indices = {
   0, 1, 2, 2, 3, 0
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.740063, 0.742313, 0.733934))));

  // ceiling
  // clang-format off
  vertices = {
   Vec3(556.0, 548.8, 0.0),
   Vec3(556.0, 548.8, 559.2),
   Vec3(0.0, 548.8, 559.2),
   Vec3(0.0, 548.8, 0.0)
  };
  indices = {
   0, 1, 2, 2, 3, 0
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.740063, 0.742313, 0.733934))));

  // back wall
  // clang-format off
  vertices = {
   Vec3(549.6, 0.0, 559.2),
   Vec3(0.0, 0.0, 559.2),
   Vec3(0.0, 548.8, 559.2),
   Vec3(556.0, 548.8, 559.2)
  };
  indices = {
   0, 1, 2, 2, 3, 0
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.740063, 0.742313, 0.733934))));

  // left wall
  // clang-format off
  vertices = {
   Vec3(552.8, 0.0, 0.0),
   Vec3(549.6, 0.0, 559.2),
   Vec3(556.0, 548.8, 559.2),
   Vec3(556.0, 548.8, 0.0)
  };
  indices = {
   0, 1, 2, 2, 3, 0
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.366046, 0.0371827, 0.0416385))));

  // right wall
  // clang-format off
  vertices = {
   Vec3(0.0, 0.0, 559.2),
   Vec3(0.0, 0.0, 0.0),
   Vec3(0.0, 548.8, 0.0),
   Vec3(0.0, 548.8, 559.2)
  };
  indices = {
   0, 1, 2, 2, 3, 0
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.162928, 0.408903, 0.0833759))));

  // clang-format off
  vertices = {
    Vec3(343.0, 548.7, 227.0),
    Vec3(343.0, 548.7, 332.0),
    Vec3(213.0, 548.7, 332.0),
    Vec3(213.0, 548.7, 227.0)
  };
  indices = {
    0, 1, 2, 2, 3, 0
  };
  // clang-format on
  auto light_geometry =
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices);
  auto area_light = std::make_unique<AreaLight>(
      Mat4(), Vec3(40, 30.902, 22.4314), std::move(light_geometry));
  auto ptr_area_light = area_light.get();
  scene.AddLight(std::move(area_light));
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.78, 0.78, 0.78)),
      ptr_area_light));
  return scene;
}

Scene SceneFactory::CboxBlocks() {
  Scene scene = Cbox();
  std::vector<Vec3> vertices;
  std::vector<int> indices;
  // short block
  // clang-format off
   vertices = {
    Vec3(130.0, 165.0, 65.0),
    Vec3(82.0, 165.0, 225.0),
    Vec3(240.0, 165.0, 272.0),
    Vec3(290.0, 165.0, 114.0),

    Vec3(290.0, 0.0, 114.0),
    Vec3(290.0, 165.0, 114.0),
    Vec3(240.0, 165.0, 272.0),
    Vec3(240.0, 0.0, 272.0),

    Vec3(130.0, 0.0, 65.0),
    Vec3(130.0, 165.0, 65.0),
    Vec3(290.0, 165.0, 114.0),
    Vec3(290.0, 0.0, 114.0),

    Vec3(82.0, 0.0, 225.0),
    Vec3(82.0, 165.0, 225.0),
    Vec3(130.0, 165.0, 65.0),
    Vec3(130.0, 0.0, 65.0),

    Vec3(240.0, 0.0, 272.0),
    Vec3(240.0, 165.0, 272.0),
    Vec3(82.0, 165.0, 225.0),
    Vec3(82.0, 0.0, 225.0)
  };
   indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.8, 0.8, 0.8))));

  // tall block
  // clang-format off
  vertices = {
   Vec3(423.0, 330.0, 247.0),
   Vec3(265.0, 330.0, 296.0),
   Vec3(314.0, 330.0, 456.0),
   Vec3(472.0, 330.0, 406.0),

   Vec3(423.0, 0.0, 247.0),
   Vec3(423.0, 330.0, 247.0),
   Vec3(472.0, 330.0, 406.0),
   Vec3(472.0, 0.0, 406.0),

   Vec3(472.0, 0.0, 406.0),
   Vec3(472.0, 330.0, 406.0),
   Vec3(314.0, 330.0, 456.0),
   Vec3(314.0, 0.0, 456.0),

   Vec3(314.0, 0.0, 456.0),
   Vec3(314.0, 330.0, 456.0),
   Vec3(265.0, 330.0, 296.0),
   Vec3(265.0, 0.0, 296.0),

   Vec3(265.0, 0.0, 296.0),
   Vec3(265.0, 330.0, 296.0),
   Vec3(423.0, 330.0, 247.0),
   Vec3(423.0, 0.0, 247.0)
  };
  indices = {
   0, 1, 2, 2, 3, 0,
   4, 5, 6, 6, 7, 4,
   8, 9, 10, 10, 11, 8,
   12, 13, 14, 14, 15, 12,
   16, 17, 18, 18, 19, 16
  };
  // clang-format on
  scene.AddObject(std::make_unique<Object>(
      std::make_unique<TriangleMesh>(Mat4(), vertices, indices),
      std::make_unique<LambertianBrdf>(Vec3(0.8, 0.8, 0.8))));
  return scene;
}

Scene SceneFactory::CboxSpheres() {
  auto cbox = Cbox();
  cbox.AddObject(std::make_unique<Object>(
      std::make_unique<Sphere>(Translate(Mat4(), Vec3(110, 100, 300)), 100),
      std::make_unique<PhongBrdf>(Vec3(0.1, 0.1, 0.1), Vec3(0.8, 0.8, 0.8),
                                  1000)));
  cbox.AddObject(std::make_unique<Object>(
      std::make_unique<Sphere>(Translate(Mat4(), Vec3(320, 120, 200)), 120),
      std::make_unique<SpecularReflectionTransmission>(1, 1.5)));
  return cbox;
}

Scene SceneFactory::CboxSphereInside() {
  auto cbox = Cbox();
  cbox.AddObject(std::make_unique<Object>(
      std::make_unique<Sphere>(Translate(Mat4(), Vec3(225, 150, 225)), 75),
      std::make_unique<LambertianBrdf>(Vec3(0.5, 0.5, 0.5))));
  cbox.AddObject(std::make_unique<Object>(
      std::make_unique<Sphere>(Translate(Mat4(), Vec3(225, 150, 225)), 150),
      std::make_unique<SpecularReflectionTransmission>(1, 1.5)));
  return cbox;
}

Scene SceneFactory::CboxBlocksDisk() {
  auto cbox = CboxBlocks();
  cbox.AddObject(std::make_unique<Object>(
      std::make_unique<Disk>(Translate(Rotate(Mat4(), M_PI / 2, Vec3(1, 0, 0)),
                                       Vec3(250, 150, 50)),
                             90),
      std::make_unique<SpecularReflectionTransmission>(1, 1.5)));
  return cbox;
}
