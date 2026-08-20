#ifndef GEOCUBE_RENDER_TEXTURE_LOADER_H
#define GEOCUBE_RENDER_TEXTURE_LOADER_H

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace geocube::render {

struct ImageRgba {
  int width = 0;
  int height = 0;
  std::vector<std::uint8_t> pixels;
};

bool loadBmpRgba(const std::filesystem::path& path, ImageRgba& image,
                 std::string& error);

} // namespace geocube::render

#endif
