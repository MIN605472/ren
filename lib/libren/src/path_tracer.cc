#include "ren/path_tracer.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include "ren/rng.h"

using namespace ren;

PathTracer::PathTracer(Scene *scene, PinholeCamera *camera, int spp)
    : scene_(scene), camera_(camera), spp_(spp) {}

void PathTracer::Render() {
  std::vector<std::thread> threads;
  auto num_threads = std::thread::hardware_concurrency();
  // num_threads = 1;
  auto dy = camera_->film().image_height() / num_threads;
  for (int i = 0; i < num_threads; ++i) {
    int min_y = i * dy;
    int max_y = (i + 1) * dy;
    threads.push_back(
        std::thread(&PathTracer::RenderRange, this, min_y, max_y));
  }

  for (auto &t : threads) {
    t.join();
  }
  camera_->film().SaveAsPpm();
}

void PathTracer::RenderRange(int min_y, int max_y) {
  for (int i = min_y; i < max_y; ++i) {
    for (int j = 0; j < camera_->film().image_width(); ++j) {
      // for (int i = 353; i < 377; ++i) {
      //   for (int j = 312; j < 369; ++j) {
      Vec3 total;
      for (int spp = 0; spp < spp_; ++spp) {
        total += Li(i, j);
      }
      camera_->film().Colorize(i, j, total / spp_);
    }
  }
}

Vec3 PathTracer::Li(int i, int j) {
  Vec3 total_rays;
  auto rays = camera_->GenRays(i, j);
  for (auto ray : rays) {
    Vec3 acc_geo_brdf(1);
    Vec3 total;
    bool previous_bounce_was_specular = false;
    for (int bounces = 0;; ++bounces) {
      SurfaceDiff surface;
      if (!scene_->Intersect(ray, surface)) {
        break;
      }
      if ((bounces == 0 || previous_bounce_was_specular) &&
          surface.o->area_light() != nullptr) {
        SurfaceDiff surface_tmp;
        surface_tmp.p = ray.origin();
        total +=
            surface.o->area_light()->L(surface_tmp, surface) * acc_geo_brdf;
      }
      auto ld = EstimateDirectRadiance(*scene_, surface, -ray.direction());
      total += acc_geo_brdf * ld;
      Vec3 sampled_wi;
      Real pdf;
      auto brdf =
          surface.o->bsdf().SampleF(surface, -ray.direction(), sampled_wi, pdf);
      if (pdf == 0 || IsZero(brdf)) {
        break;
      }
      acc_geo_brdf *= brdf * std::abs(Dot(surface.y, sampled_wi)) / pdf;
      if (bounces > 4) {
        Real end_probability = std::max(Real(0.1), 1.0 - MaxComp(acc_geo_brdf));
        if (rng::Uniform() < end_probability) {
          break;
        }
        acc_geo_brdf /= 1.0 - end_probability;
      }
      previous_bounce_was_specular =
          surface.o->bsdf().type_ & Bsdf::Type::kSpecular;
      auto push = Dot(surface.y, sampled_wi) < 0 ? -surface.y : surface.y;
      ray = Ray(surface.p + 1E-4 * push, sampled_wi);
    }
    total_rays += total;
  }
  return total_rays / rays.size();
}
