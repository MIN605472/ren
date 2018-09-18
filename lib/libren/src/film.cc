#define TINYEXR_IMPLEMENTATION
#include "ren/film.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include "ren/typedefs.h"

using namespace ren;

Film::Film(Real film_height, Real film_width, Real image_height,
           Real image_width, const std::string &path)
    : film_height_(film_height),
      film_width_(film_width),
      image_height_(image_height),
      image_width_(image_width),
      image_(image_height * image_width),
      path_(path) {}

void Film::SaveAsPpm() const {
  std::ofstream file;
  file.open(path_ + ".ppm");
  file << "P3\n";
  file << image_width_ << " " << image_height_ << "\n";
  file << "255\n";
  for (int i = 0; i < image_height_; ++i) {
    for (int j = 0; j < image_width_ - 1; ++j) {
      auto color = Clamp(image_[index(i, j)], Real(0), Real(1));
      file << static_cast<int>(color.x * 255) << " ";
      file << static_cast<int>(color.y * 255) << " ";
      file << static_cast<int>(color.z * 255) << " ";
    }
    auto color = Clamp(image_[index(i, image_width_ - 1)], Real(0), Real(1));
    file << static_cast<int>(color.x * 255) << " ";
    file << static_cast<int>(color.y * 255) << " ";
    file << static_cast<int>(color.z * 255) << "\n";
  }
  file.close();
}

void Film::Colorize(int row, int col, const Vec3 &c) {
  image_[index(row, col)] = c;
}

Real Film::film_height() const { return film_height_; }

Real Film::film_width() const { return film_width_; }

int Film::image_height() const { return image_height_; }

int Film::image_width() const { return image_width_; }
