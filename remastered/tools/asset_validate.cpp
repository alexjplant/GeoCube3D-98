#include "platform/asset_manifest.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

bool hasPrefix(const std::filesystem::path& path, const std::string& prefix)
{
  std::ifstream input(path, std::ios::binary);
  if (!input)
    return false;
  std::string value(prefix.size(), '\0');
  input.read(value.data(), static_cast<std::streamsize>(value.size()));
  return value == prefix;
}

bool isBmp(const std::filesystem::path& path)
{
  std::ifstream input(path, std::ios::binary);
  char signature[2]{};
  return input && input.read(signature, 2) && signature[0] == 'B' &&
         signature[1] == 'M';
}

bool isRemasteredMesh(const std::filesystem::path& path)
{
  return hasPrefix(path, "geocube_mesh 1");
}

bool isSoundFont(const std::filesystem::path& path)
{
  std::ifstream input(path, std::ios::binary);
  char header[12]{};
  return input && input.read(header, sizeof(header)) &&
         std::string(header, header + 4) == "RIFF" &&
         std::string(header + 8, header + 12) == "sfbk";
}

bool isRegular(const std::filesystem::path& path)
{
  std::error_code error;
  return std::filesystem::is_regular_file(path, error);
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: geocube3d_asset_validate <asset-root>\n";
    return 2;
  }

  const std::filesystem::path root(argv[1]);
  bool valid = true;
  for (const auto& asset : geocube::platform::kAssetManifest) {
    if (!asset.native)
      continue;
    const std::filesystem::path path = root / asset.path;
    bool assetValid = isRegular(path);
    switch (asset.kind) {
    case geocube::platform::AssetKind::Mesh:
      assetValid = assetValid && isRemasteredMesh(path);
      break;
    case geocube::platform::AssetKind::Wave:
      assetValid = assetValid && hasPrefix(path, "RIFF");
      break;
    case geocube::platform::AssetKind::Midi:
      assetValid = assetValid && hasPrefix(path, "MThd");
      break;
    case geocube::platform::AssetKind::Bitmap:
      assetValid = assetValid && isBmp(path);
      break;
    case geocube::platform::AssetKind::SoundFont:
      assetValid = assetValid && isSoundFont(path);
      break;
    case geocube::platform::AssetKind::Obj:
    case geocube::platform::AssetKind::Mtl:
    case geocube::platform::AssetKind::Ogg:
      break;
    }
    if (!assetValid) {
      std::cerr << "Invalid or missing asset: " << asset.path << '\n';
      valid = false;
    }
  }

  if (valid)
    std::cout << "Remastered asset validation passed\n";
  return valid ? 0 : 1;
}
