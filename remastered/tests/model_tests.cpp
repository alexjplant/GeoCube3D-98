#include "render/model_loader.h"

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace {

bool near(float actual, float expected)
{
  return std::fabs(actual - expected) < 0.0001f;
}

bool colorIs(const geocube::core::Vec3& color, float red, float green,
             float blue)
{
  return near(color.x, red) && near(color.y, green) && near(color.z, blue);
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: geocube3d_model_tests <models-directory>\n";
    return EXIT_FAILURE;
  }

  const std::filesystem::path root(argv[1]);
  const char* names[] = {"ship", "cube", "jack", "portal", "sphere3",
                         "pasta"};
  for (const char* name : names) {
    geocube::render::Model model;
    std::string error;
    if (!geocube::render::loadObjModel(root / (std::string(name) + ".obj"),
                                       model, error)) {
      std::cerr << error << '\n';
      return EXIT_FAILURE;
    }
    if (model.vertices.empty() || model.indices.empty() || model.groups.empty())
      return EXIT_FAILURE;
  }

  geocube::render::Model ship;
  std::string error;
  if (!geocube::render::loadObjModel(root / "ship.obj", ship, error) ||
      ship.groups.size() != 3 ||
      !colorIs(ship.groups[0].diffuse, 1.0f, 0.0f, 0.0f) ||
      !colorIs(ship.groups[1].diffuse, 0.976470649f, 1.0f, 0.0f) ||
      !colorIs(ship.groups[2].diffuse, 0.964705944f, 0.0196078438f,
               0.0196078438f)) {
    std::cerr << "Ship material regression\n";
    return EXIT_FAILURE;
  }

  geocube::render::Model cube;
  if (!geocube::render::loadObjModel(root / "cube.obj", cube, error) ||
      cube.groups.size() != 1 || !colorIs(cube.groups[0].diffuse, 1.0f, 1.0f,
                                           1.0f)) {
    std::cerr << "Cube material regression\n";
    return EXIT_FAILURE;
  }

  std::cout << "Converted model and material tests passed\n";
  return EXIT_SUCCESS;
}
