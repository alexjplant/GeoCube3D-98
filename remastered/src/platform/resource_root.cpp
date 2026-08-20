#include "platform/resource_root.h"

#include <SDL3/SDL.h>

#include <cstdlib>

namespace geocube::platform {
namespace {

void addCandidate(std::vector<std::filesystem::path>& candidates,
                  const std::filesystem::path& candidate)
{
  if (candidate.empty())
    return;

  std::error_code error;
  const std::filesystem::path normalized =
      std::filesystem::weakly_canonical(candidate, error);
  const std::filesystem::path& value = error ? candidate : normalized;

  for (const auto& existing : candidates) {
    if (existing == value)
      return;
  }
  candidates.push_back(value);
}

bool isResourceRoot(const std::filesystem::path& root)
{
  std::error_code error;
  return std::filesystem::is_directory(root / "models", error) &&
         std::filesystem::is_directory(root / "textures", error) &&
         std::filesystem::is_directory(root / "audio", error);
}

} // namespace

std::vector<std::filesystem::path> resourceRootCandidates()
{
  std::vector<std::filesystem::path> candidates;

#ifdef __EMSCRIPTEN__
  addCandidate(candidates, "/assets");
#endif

  if (const char* configured = std::getenv("GEOCUBE_ASSET_ROOT"))
    addCandidate(candidates, configured);

  const char* basePath = SDL_GetBasePath();
  if (basePath) {
    const std::filesystem::path executableDirectory(basePath);

    addCandidate(candidates, executableDirectory / "assets");
    addCandidate(candidates, executableDirectory / "../assets");
    addCandidate(candidates, executableDirectory / "../../assets");
  }

  return candidates;
}

std::filesystem::path discoverResourceRoot()
{
  for (const auto& candidate : resourceRootCandidates()) {
    if (isResourceRoot(candidate))
      return candidate;
  }
  return {};
}

bool hasAsset(const std::filesystem::path& root, std::string_view relativePath)
{
  std::error_code error;
  return std::filesystem::is_regular_file(root / relativePath, error);
}

} // namespace geocube::platform
