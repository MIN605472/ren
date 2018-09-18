#include <cstring>
#include <iostream>
#include "ren/ren.h"

using namespace ren;

static const char kUsage[] =
    R"(Ren. A small path tracer and photon mapping renderer.

    Usage:
      ren [-r <string>] [-spp <integer>] [-s <string>] [-o <string>] [-cp <integer>] [-ip <integer>] [-np <integer>]
      ren -h

    Options:
      -spp <integer> 
          Number of samples per pixel. [default: 8]

      -o <name>     
           Path of the output image without the extensions. [default: output]

      -s <cbox_blocks|cbox_spheres|cbox_sphere_inside|cbox_blocks_disk>     
           Name of the scene to render. [default: cbox_blocks]

      -r <pt|pm>    
           Method to render the scene. Choose one between <pt> (path tracing)
           and <pm> (photon mapping). [default: pt]

      -cp <integer> 
           Number of caustic photons to launch for photon mapping. [default: 10000]

      -ip <integer> 
           Number of indirect photons to launch for photon mapping. [default: 10000000]

      -np <integer> 
           Number of neighbours photons to use during radiance estimation in photon mapping. [default: 100]

      -h            
           Show this screen.
)";

Real fw = 0.025;
Real fh = 0.025;
int iw = 512;
int ih = 512;
int spp = 8;
int num_caustic_photons = 50000;
int num_indirect_photons = 10000000;
int num_neighbour_photons = 100;
std::string o = "output";
std::string s = "cbox_blocks";
std::string r = "pt";

void GetValue(int argc, char *argv[], int &option, int &value) {
  if (option + 1 < argc) {
    try {
      value = std::stoi(argv[option + 1]);
      ++option;
    } catch (const std::invalid_argument &) {
      std::string s = "Value of option \"" + std::string(argv[option]) +
                      "\" is not a number.";
      throw std::invalid_argument(s);
    }
  } else {
    std::string msg = "Option \"" + std::string(argv[option]) +
                      "\" specified without a value.";
    throw std::invalid_argument(msg);
  }
}

void GetValue(int argc, char *argv[], int &option,
              std::initializer_list<std::string> valid, std::string &value) {
  if (option + 1 < argc) {
    value = argv[option + 1];
    ++option;
    if (valid.size() != 0 &&
        std::find(valid.begin(), valid.end(), value) == valid.end()) {
      std::string msg =
          "Unkown value for option \"" + std::string(argv[option]) + "\"";
      throw std::invalid_argument(msg);
    }
  } else {
    std::string msg = "Option \"" + std::string(argv[option]) +
                      "\" specified without a value.";
    throw std::invalid_argument(msg);
  }
}

int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "-h") == 0) {
    std::cout << kUsage;
    return 0;
  }
  try {
    for (int i = 1; i < argc; ++i) {
      // if (strcmp(argv[i], "-fw") == 0) {
      //   GetValue(argc, argv, i, fw);
      // } else if (strcmp(argv[i], "-fh") == 0) {
      //   GetValue(argc, argv, i, fh);
      // } else if (strcmp(argv[i], "-iw") == 0) {
      //   GetValue(argc, argv, i, iw);
      // } else if (strcmp(argv[i], "-ih") == 0) {
      //   GetValue(argc, argv, i, ih);
      // }
      if (strcmp(argv[i], "-spp") == 0) {
        GetValue(argc, argv, i, spp);
      } else if (strcmp(argv[i], "-cp") == 0) {
        GetValue(argc, argv, i, num_caustic_photons);
      } else if (strcmp(argv[i], "-ip") == 0) {
        GetValue(argc, argv, i, num_indirect_photons);
      } else if (strcmp(argv[i], "-np") == 0) {
        GetValue(argc, argv, i, num_neighbour_photons);
      } else if (strcmp(argv[i], "-o") == 0) {
        GetValue(argc, argv, i, {}, o);
      } else if (strcmp(argv[i], "-s") == 0) {
        GetValue(argc, argv, i, {}, s);
      } else if (strcmp(argv[i], "-r") == 0) {
        GetValue(argc, argv, i, {"pt", "pm"}, r);
      } else {
        std::string msg = "Unknown option \"" + std::string(argv[i]) + "\".";
        throw std::invalid_argument(msg);
      }
    }
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what();
    std::cerr << "\nSee \"" + std::string(argv[0]) + " -h\" for help\n";
    return -1;
  }
  auto scene = SceneFactory::GetInstance().GetScene(s);
  if (scene == nullptr) {
    std::cerr << "The scene \"" + s + "\" doesn't exist\n";
    return -1;
  }
  Film film(fh, fw, ih, iw, o);
  PinholeCamera camera(Vec3(278, 273, -800), Vec3(278, 273, 0.0),
                       Vec3(0.0, 1.0, 0.0), 0.035, film);
  std::unique_ptr<Renderer> renderer;
  if (r == "pt") {
    renderer = std::make_unique<PathTracer>(scene, &camera, spp);
  } else {
    renderer = std::make_unique<PhotonMapper>(
        scene, &camera, spp, num_caustic_photons, num_indirect_photons,
        num_neighbour_photons);
  }
  renderer->Render();
  return 0;
}
