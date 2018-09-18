#ifndef REN_PINHOLECAMERA_H_
#define REN_PINHOLECAMERA_H_
#include <vector>
#include "ren/film.h"
#include "ren/mat.h"
#include "ren/ray.h"
#include "ren/scene.h"
#include "ren/vec.h"
namespace ren {
class PinholeCamera {
 public:
  // Construct a simple pinhole camera
  // @param from the point from where the camera is looking
  // @param to the point to where the camera is looking at
  // @param up the orientation of the camera specified by the up direction
  // @param focal_length the distance between the pinhole and the film
  // @param film the description of the photographic film and digital image
  PinholeCamera(const Vec3 &from, const Vec3 &to, const Vec3 &up,
                Real focal_length, const Film &film);
  Film &film();
  // Generate a single ray.
  // @param col the height coordinate of the image
  // @param row the width coordinate of the image
  // @return ray in world space coordinates shot from the camera position the
  // middle of the pixel (\p col, \p row)
  Ray GenRay(int col, int row);
  // Generate a number of rays. This is used for antialiasing.
  // @param col the height coordinate of the image
  // @param row the width coordinate of the image
  // @return a couple of rays for the pixel (\p col, \p row)
  std::vector<Ray> GenRays(int col, int row);

 private:
  Mat4 camera_to_world_;
  Vec3 d_x_;
  Vec3 d_y_;
  Vec3 offset_;
  Vec3 top_left_film_;
  const Scene *scene_;
  Film film_;
};
}  // namespace ren
#endif  // REN_PINHOLECAMERA_H_
