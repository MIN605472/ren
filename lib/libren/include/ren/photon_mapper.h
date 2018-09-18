#ifndef REN_PHOTONMAPPER_H_
#define REN_PHOTONMAPPER_H_
#include "ren/photon_map.h"
#include "ren/pinhole_camera.h"
#include "ren/renderer.h"
#include "ren/scene.h"
namespace ren {
// Photon mapping renderer.
class PhotonMapper : public Renderer {
 public:
  PhotonMapper(Scene *scene, PinholeCamera *camera, int spp,
               int num_caustic_photons, int num_indirect_photons,
               int num_neighbour_photons);
  virtual void Render() override;

 private:
  PhotonMap BuildPhotonMap(const Scene &scene);
  void RenderRange(int min_y, int max_y, const PhotonMap &photon_map);
  Vec3 Li(int i, int j, const PhotonMap &photon_map,
          std::vector<PhotonMap::QueryResult> &query_results);
  Scene *scene_;
  PinholeCamera *camera_;
  int num_sampled_photons_;
  int spp_;
  int num_caustic_photons_;
  int num_indirect_photons_;
  int num_neighbour_photons_;
};

}  // namespace ren
#endif  // REN_PHOTONMAPPER_H_
