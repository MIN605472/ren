#include "ren/pinhole_camera.h"
#include <iostream>
#include "ren/rng.h"
#include "ren/transform.h"

using namespace ren;

PinholeCamera::PinholeCamera(const Vec3 &from, const Vec3 &to, const Vec3 &up,
                             Real focal_length, const Film &film)
    : film_(film) {
  camera_to_world_ = LookAt(from, to, up);
  top_left_film_.x = -film.film_width() / 2.0;
  top_left_film_.y = film.film_height() / 2.0;
  top_left_film_.z = -focal_length;
  Real change_in_x = film.film_width() / film.image_width();
  Real change_in_y = film.film_height() / film.image_height();
  d_x_ = Vec3(change_in_x, 0, 0);
  d_y_ = Vec3(0, -change_in_y, 0);
  offset_ = Vec3(change_in_x / 2.0, -change_in_y / 2.0, 0);
}

Film &PinholeCamera::film() { return film_; }

Ray PinholeCamera::GenRay(int row, int col) {
  Vec3 dir(top_left_film_ + d_x_ * col + d_y_ * row + offset_);
  dir = Normalize(dir);
  return Ray(dir).Transform(camera_to_world_);
}

std::vector<Ray> PinholeCamera::GenRays(int row, int col) {
  std::vector<Ray> rays;
  auto new_dx = d_x_ / 2.0;
  auto new_dy = d_y_ / 2.0;
  std::vector<Vec3> samples;
  for (int i = 0; i < 4; ++i) {
    samples.emplace_back(rng::Uniform() * new_dx.x, rng::Uniform() * new_dy.y,
                         Real(0));
  }
  Vec3 dir(top_left_film_ + d_x_ * col + d_y_ * row);
  int sample = 0;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      Ray r(Normalize(dir + new_dx * j + new_dy * i + samples[sample]));
      ++sample;
      rays.push_back(r.Transform(camera_to_world_));
    }
  }
  return rays;
}
