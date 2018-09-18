#include "ren/renderer.h"

using namespace ren;

Vec3 Renderer::EstimateDirectRadiance(const Scene &scene,
                                      const SurfaceDiff &surface,
                                      const Vec3 &wo, int samples) {
  Vec3 total;
  for (const auto &light : scene.lights()) {
    for (int i = 0; i < samples; ++i) {
      SurfaceDiff surface_light;
      Real pdf;
      auto radiance = light->SampleLi(surface, surface_light, pdf);
      if (pdf == 0 || IsZero(radiance)) {
        continue;
      }
      auto wi = surface_light.p - surface.p;
      auto length = Length(wi);
      wi = Normalize(wi);
      Ray r(surface.p + 1E-4 * surface.y, wi, length * (1 - 1E-4));
      SurfaceDiff tmp_surface;
      if (!scene.Intersect(r, tmp_surface)) {
        total += radiance * std::abs(Dot(wi, surface.y)) *
                 surface.o->bsdf().F(surface, wo, wi) / pdf;
      }
    }
    total /= samples;
  }
  return total;
}
