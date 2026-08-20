#ifndef GEOCUBE_RENDER_MODEL_LOADER_H
#define GEOCUBE_RENDER_MODEL_LOADER_H

#include "core/math.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace geocube::render {

struct ModelGroup {
  std::vector<std::uint32_t> indices;
  core::Vec3 diffuse{1.0f, 1.0f, 1.0f};
};

struct Model {
  std::vector<core::Vec3> vertices;
  std::vector<std::uint32_t> indices;
  std::vector<ModelGroup> groups;
};

bool loadRemasteredMesh(const std::filesystem::path& path, Model& model,
                        std::string& error);
bool loadObjModel(const std::filesystem::path& path, Model& model,
                  std::string& error);

} // namespace geocube::render

#endif
