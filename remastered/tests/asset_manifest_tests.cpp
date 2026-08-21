#include "platform/asset_manifest.h"

#include <cstdlib>
#include <iostream>
#include <string_view>

int main()
{
  using namespace geocube::platform;
  if (kAssetManifest.size() != 42) {
    std::cerr << "Unexpected asset manifest size\n";
    return EXIT_FAILURE;
  }

  std::size_t native = 0;
  std::size_t web = 0;
  for (const auto& asset : kAssetManifest) {
    if (asset.path.empty() || (asset.native == false && asset.web == false)) {
      std::cerr << "Invalid asset manifest entry\n";
      return EXIT_FAILURE;
    }
    native += asset.native ? 1 : 0;
    web += asset.web ? 1 : 0;
  }
  if (native != 35 || web != 34) {
    std::cerr << "Unexpected platform asset counts\n";
    return EXIT_FAILURE;
  }

  std::cout << "Asset manifest tests passed\n";
  return EXIT_SUCCESS;
}
