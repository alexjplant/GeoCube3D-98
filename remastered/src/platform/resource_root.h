#ifndef GEOCUBE_REMASTERED_RESOURCE_ROOT_H
#define GEOCUBE_REMASTERED_RESOURCE_ROOT_H

#include <filesystem>
#include <string_view>
#include <vector>

namespace geocube::platform {

std::vector<std::filesystem::path> resourceRootCandidates();
std::filesystem::path discoverResourceRoot();
bool hasAsset(const std::filesystem::path& root, std::string_view relativePath);

} // namespace geocube::platform

#endif
