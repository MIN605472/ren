#define _USE_MATH_DEFINES
#include "ren/photon_mapper.h"
#include <fstream>
#include <thread>
#include "ren/rng.h"
#include "ren/sampling.h"
#include "ren/scene.h"
#include "ren/transform.h"

using namespace ren;

PhotonMapper::PhotonMapper(Scene *scene, PinholeCamera *camera, int spp,
                           int num_caustic_photons, int num_indirect_photons,
                           int num_neighbour_photons)
    : scene_(scene),
      camera_(camera),
      num_sampled_photons_(0),
      spp_(spp),
      num_caustic_photons_(num_caustic_photons),
      num_indirect_photons_(num_indirect_photons),
      num_neighbour_photons_(num_neighbour_photons) {}

void PhotonMapper::Render() {
  auto photon_map = BuildPhotonMap(*scene_);
  std::vector<std::thread> threads;
  auto num_threads = std::thread::hardware_concurrency();
  auto dy = camera_->film().image_height() / num_threads;
  for (int i = 0; i < num_threads; ++i) {
    int min_y = i * dy;
    int max_y = (i + 1) * dy;
    threads.push_back(std::thread(&PhotonMapper::RenderRange, this, min_y,
                                  max_y, std::ref(photon_map)));
  }
  for (auto &t : threads) {
    t.join();
  }
  camera_->film().SaveAsPpm();
}

PhotonMap PhotonMapper::BuildPhotonMap(const Scene &scene) {
  int num_caustic_photons_generated = 0;
  int num_indirect_photons_generated = 0;
  int num_sampled_photons = 0;
  std::vector<Photon> caustic_photons;
  std::vector<Photon> indirect_photons;
  caustic_photons.reserve(num_caustic_photons_);
  indirect_photons.reserve(num_indirect_photons_ + num_caustic_photons_);
  bool has_any_specular_object = scene.AnyObjectWithBsdf(
      Bsdf::Type(Bsdf::Type::kSpecular | Bsdf::Type::kTransmissive));
  bool has_any_diffuse_object = scene.AnyObjectWithBsdf(
      Bsdf::Type(Bsdf::Type::kDiffuse | Bsdf::Type::kReflective));
  while ((has_any_specular_object &&
          num_caustic_photons_generated < num_caustic_photons_) ||
         (has_any_diffuse_object &&
          num_indirect_photons_generated < num_indirect_photons_)) {
    // TODO: this should be changed later on if multiple lights is to
    // be taken into account; sampling lights based on the emitted
    // power should be enough
    for (const auto &light : scene.lights()) {
      Real pdf_dir;
      Real pdf_point;
      Vec3 acc(1);
      SurfaceDiff sampled_point;
      Vec3 dir;
      auto le = light->SampleLe(sampled_point, dir, pdf_point, pdf_dir);
      ++num_sampled_photons;
      acc = le * Dot(sampled_point.y, dir) / pdf_dir / pdf_point;
      Ray ray(sampled_point.p + 1E-4 * sampled_point.y, dir);
      int bounces = 0;
      bool previous_bounce_was_specular = false;
      for (;;) {
        SurfaceDiff surface_diff;
        if (!scene.Intersect(ray, surface_diff)) {
          break;
        }
        ++bounces;
        if (bounces > 1 &&
            surface_diff.o->bsdf().type_ & Bsdf::Type::kDiffuse) {
          Photon p(surface_diff.p, acc, -dir);
          if (previous_bounce_was_specular &&
              num_caustic_photons_generated < num_caustic_photons_) {
            ++num_caustic_photons_generated;
            caustic_photons.push_back(p);
          } else if (num_indirect_photons_generated < num_indirect_photons_) {
            ++num_indirect_photons_generated;
            indirect_photons.push_back(p);
          }
        }
        Vec3 new_dir;
        Real pdf;
        auto bsdf = surface_diff.o->bsdf().SampleF(surface_diff, -dir, new_dir,
                                                   pdf, true);
        if (pdf == 0 || IsZero(bsdf)) {
          break;
        }
        previous_bounce_was_specular =
            surface_diff.o->bsdf().type_ & Bsdf::Type::kSpecular;
        auto cos_theta_o = Dot(new_dir, surface_diff.y);
        auto acc_new = acc * bsdf * std::abs(cos_theta_o) / pdf;
        auto push_dir = cos_theta_o < 0 ? -surface_diff.y : surface_diff.y;
        ray = Ray(surface_diff.p + 1E-4 * push_dir, new_dir);
        auto survival_probability =
            std::min(Real(1), MaxComp(acc_new) / MaxComp(acc));
        if (rng::Uniform() > survival_probability) {
          break;
        }
        acc = acc_new / survival_probability;
      }
    }
  }
  num_sampled_photons_ = num_sampled_photons;
  indirect_photons.insert(indirect_photons.end(), caustic_photons.begin(),
                          caustic_photons.end());
  return PhotonMap(indirect_photons);
}

void PhotonMapper::RenderRange(int min_y, int max_y,
                               const PhotonMap &photon_map) {
  std::vector<PhotonMap::QueryResult> query_results;
  query_results.reserve(num_neighbour_photons_);
  for (int i = min_y; i < max_y; ++i) {
    for (int j = 0; j < camera_->film().image_width(); ++j) {
      Vec3 total;
      for (int spp = 0; spp < spp_; ++spp) {
        total += Li(i, j, photon_map, query_results);
      }
      camera_->film().Colorize(i, j, total / Real(spp_));
    }
  }
}

Vec3 PhotonMapper::Li(int i, int j, const PhotonMap &photon_map,
                      std::vector<PhotonMap::QueryResult> &query_results) {
  Vec3 total_rays;
  auto rays = camera_->GenRays(i, j);
  for (auto ray : rays) {
    Vec3 total;
    Vec3 throughput(1);
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
        total += surface.o->area_light()->L(surface_tmp, surface) * throughput;
      }
      if (surface.o->bsdf().type_ & Bsdf::Type::kDiffuse) {
        photon_map.QueryNearest(surface.p, num_neighbour_photons_,
                                query_results);
        Vec3 total_flux;
        auto radius = query_results.front().distance2;
        for (const auto &query_result : query_results) {
          total_flux += surface.o->bsdf().F(surface, -ray.direction(),
                                            query_result.data.dir) *
                        query_result.data.power;
        }
        total += EstimateDirectRadiance(*scene_, surface, -ray.direction()) *
                 throughput;
        total += total_flux / 2.0 / M_PI / radius / num_sampled_photons_ *
                 throughput;
        break;
      }
      total += EstimateDirectRadiance(*scene_, surface, -ray.direction()) *
               throughput;
      Vec3 sampled_wi;
      Real pdf;
      auto bsdf =
          surface.o->bsdf().SampleF(surface, -ray.direction(), sampled_wi, pdf);
      if (pdf == 0 || IsZero(bsdf)) {
        break;
      }
      auto cos_theta_i = Dot(surface.y, sampled_wi);
      throughput *= bsdf * std::abs(cos_theta_i) / pdf;
      auto push_dir = cos_theta_i < 0 ? -surface.y : surface.y;
      ray = Ray(surface.p + 1E-4 * push_dir, sampled_wi);
      previous_bounce_was_specular =
          surface.o->bsdf().type_ & Bsdf::Type::kSpecular;
      if (bounces > 4) {
        Real end_probability = std::max(Real(0.1), 1.0 - MaxComp(throughput));
        if (rng::Uniform() < end_probability) {
          break;
        }
        throughput /= 1.0 - end_probability;
      }
    }
    total_rays += total;
  }
  return total_rays / rays.size();
}
