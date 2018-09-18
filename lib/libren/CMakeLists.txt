cmake_minimum_required(VERSION 2.8)
project(libren)
set(HDRS 
  include/ren/ren.h
  include/ren/typedefs.h
  include/ren/film.h
  include/ren/pinhole_camera.h
  include/ren/light.h
  include/ren/point_light.h
  include/ren/area_light.h 
  include/ren/mat.h
  include/ren/vec.h 
  include/ren/transform.h
  include/ren/object.h 
  include/ren/triangle.h
  include/ren/plane.h
  include/ren/ray.h
  include/ren/scene.h
  include/ren/shape.h 
  include/ren/sphere.h
  include/ren/disk.h 
  include/ren/bsdf.h 
  include/ren/surface_diff.h
  include/ren/renderer.h
  include/ren/path_tracer.h
  include/ren/scene_factory.h 
  include/ren/rng.h
  include/ren/photon_map.h
  include/ren/photon_mapper.h
  include/ren/sampling.h)
set(SRCS 
  src/film.cc 
  src/pinhole_camera.cc
  src/point_light.cc 
  src/light.cc 
  src/area_light.cc 
  src/object.cc 
  src/plane.cc
  src/ray.cc
  src/triangle.cc
  src/scene.cc
  src/shape.cc
  src/disk.cc
  src/sphere.cc
  src/surface_diff.cc 
  src/bsdf.cc 
  src/path_tracer.cc 
  src/scene_factory.cc 
  src/rng.cc
  src/photon_mapper.cc
  src/renderer.cc
  src/sampling.cc)

add_library(${PROJECT_NAME} ${HDRS} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(${PROJECT_NAME} PUBLIC -pthread)
