#include "render/model_loader.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace geocube::render {

bool loadRemasteredMesh(const std::filesystem::path& path, Model& model,
                        std::string& error)
{
  std::ifstream input(path);
  if (!input) {
    error = "could not open " + path.string();
    return false;
  }

  std::string header;
  int version = 0;
  if (!(input >> header >> version) || header != "geocube_mesh" ||
      version != 1) {
    error = "unsupported remastered mesh header in " + path.string();
    return false;
  }

  std::string section;
  std::size_t vertexCount = 0;
  if (!(input >> section >> vertexCount) || section != "vertices") {
    error = "missing vertex section in " + path.string();
    return false;
  }

  Model loaded;
  loaded.vertices.reserve(vertexCount);
  for (std::size_t i = 0; i < vertexCount; ++i) {
    core::Vec3 vertex;
    if (!(input >> vertex.x >> vertex.y >> vertex.z)) {
      error = "invalid vertex data in " + path.string();
      return false;
    }
    loaded.vertices.push_back(vertex);
  }

  std::size_t triangleCount = 0;
  if (!(input >> section >> triangleCount) || section != "triangles") {
    error = "missing triangle section in " + path.string();
    return false;
  }

  loaded.indices.reserve(triangleCount * 3);
  for (std::size_t i = 0; i < triangleCount; ++i) {
    std::uint32_t first = 0;
    std::uint32_t second = 0;
    std::uint32_t third = 0;
    if (!(input >> first >> second >> third) || first >= vertexCount ||
        second >= vertexCount || third >= vertexCount) {
      error = "invalid triangle data in " + path.string();
      return false;
    }
    loaded.indices.push_back(first);
    loaded.indices.push_back(second);
    loaded.indices.push_back(third);
  }

  if (loaded.vertices.empty() || loaded.indices.empty()) {
    error = "remastered mesh is empty: " + path.string();
    return false;
  }

  loaded.groups.push_back({loaded.indices, {1.0f, 1.0f, 1.0f}});
  model = std::move(loaded);
  return true;
}

namespace {

using MaterialMap = std::unordered_map<std::string, core::Vec3>;

MaterialMap loadMaterials(const std::filesystem::path& path)
{
  MaterialMap materials;
  std::ifstream input(path);
  std::string line;
  std::string current;
  while (std::getline(input, line)) {
    std::istringstream tokens(line);
    std::string directive;
    tokens >> directive;
    if (directive == "newmtl") {
      tokens >> current;
      materials[current] = {1.0f, 1.0f, 1.0f};
    } else if (directive == "Kd" && !current.empty()) {
      tokens >> materials[current].x >> materials[current].y >>
          materials[current].z;
    }
  }
  return materials;
}

bool parseObjIndex(const std::string& token, std::size_t vertexCount,
                   std::uint32_t& index)
{
  const std::size_t slash = token.find('/');
  const std::string position = token.substr(0, slash);
  if (position.empty())
    return false;
  try {
    const int parsed = std::stoi(position);
    const int resolved = parsed < 0
                             ? static_cast<int>(vertexCount) + parsed
                             : parsed - 1;
    if (resolved < 0 || resolved >= static_cast<int>(vertexCount))
      return false;
    index = static_cast<std::uint32_t>(resolved);
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

} // namespace

bool loadObjModel(const std::filesystem::path& path, Model& model,
                  std::string& error)
{
  std::ifstream input(path);
  if (!input) {
    error = "could not open " + path.string();
    return false;
  }

  MaterialMap materials;
  Model loaded;
  core::Vec3 currentDiffuse{1.0f, 1.0f, 1.0f};
  loaded.groups.push_back({{}, currentDiffuse});
  std::string line;
  while (std::getline(input, line)) {
    std::istringstream tokens(line);
    std::string directive;
    tokens >> directive;
    if (directive.empty() || directive[0] == '#')
      continue;
    if (directive == "mtllib") {
      std::string materialFile;
      tokens >> materialFile;
      materials = loadMaterials(path.parent_path() / materialFile);
    } else if (directive == "usemtl") {
      std::string materialName;
      tokens >> materialName;
      const auto material = materials.find(materialName);
      currentDiffuse = material == materials.end()
                           ? core::Vec3{1.0f, 1.0f, 1.0f}
                           : material->second;
      if (!loaded.groups.back().indices.empty())
        loaded.groups.push_back({{}, currentDiffuse});
      else
        loaded.groups.back().diffuse = currentDiffuse;
    } else if (directive == "v") {
      core::Vec3 vertex;
      if (!(tokens >> vertex.x >> vertex.y >> vertex.z)) {
        error = "invalid OBJ vertex in " + path.string();
        return false;
      }
      loaded.vertices.push_back(vertex);
    } else if (directive == "f") {
      std::vector<std::uint32_t> face;
      std::string vertexToken;
      while (tokens >> vertexToken) {
        std::uint32_t index = 0;
        if (!parseObjIndex(vertexToken, loaded.vertices.size(), index)) {
          error = "invalid OBJ face in " + path.string();
          return false;
        }
        face.push_back(index);
      }
      if (face.size() < 3) {
        error = "OBJ face has fewer than three vertices in " + path.string();
        return false;
      }
      for (std::size_t i = 1; i + 1 < face.size(); ++i) {
        loaded.indices.insert(loaded.indices.end(),
                              {face[0], face[i], face[i + 1]});
        loaded.groups.back().indices.insert(loaded.groups.back().indices.end(),
                                            {face[0], face[i], face[i + 1]});
      }
    }
  }

  if (loaded.vertices.empty() || loaded.indices.empty()) {
    error = "OBJ model is empty: " + path.string();
    return false;
  }
  loaded.groups.erase(
      std::remove_if(loaded.groups.begin(), loaded.groups.end(),
                     [](const ModelGroup& group) {
                       return group.indices.empty();
                     }),
      loaded.groups.end());
  model = std::move(loaded);
  return true;
}

} // namespace geocube::render
