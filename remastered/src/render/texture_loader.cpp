#include "render/texture_loader.h"

#include <fstream>
#include <utility>

namespace geocube::render {
namespace {

std::uint16_t read16(const std::uint8_t* data)
{
  return static_cast<std::uint16_t>(data[0]) |
         (static_cast<std::uint16_t>(data[1]) << 8);
}

std::uint32_t read32(const std::uint8_t* data)
{
  return static_cast<std::uint32_t>(data[0]) |
         (static_cast<std::uint32_t>(data[1]) << 8) |
         (static_cast<std::uint32_t>(data[2]) << 16) |
         (static_cast<std::uint32_t>(data[3]) << 24);
}

std::int32_t readSigned32(const std::uint8_t* data)
{
  return static_cast<std::int32_t>(read32(data));
}

} // namespace

bool loadBmpRgba(const std::filesystem::path& path, ImageRgba& image,
                 std::string& error)
{
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    error = "could not open " + path.string();
    return false;
  }

  input.seekg(0, std::ios::end);
  const std::streamoff fileSize = input.tellg();
  input.seekg(0, std::ios::beg);
  if (fileSize < 54) {
    error = "BMP is too small: " + path.string();
    return false;
  }

  std::vector<std::uint8_t> data(static_cast<std::size_t>(fileSize));
  input.read(reinterpret_cast<char*>(data.data()), fileSize);
  if (!input || data[0] != 'B' || data[1] != 'M') {
    error = "not a BMP file: " + path.string();
    return false;
  }

  const std::uint32_t pixelOffset = read32(data.data() + 10);
  const std::uint32_t dibSize = read32(data.data() + 14);
  const std::int32_t width = readSigned32(data.data() + 18);
  const std::int32_t height = readSigned32(data.data() + 22);
  const std::uint16_t planes = read16(data.data() + 26);
  const std::uint16_t bitsPerPixel = read16(data.data() + 28);
  const std::uint32_t compression = read32(data.data() + 30);

  if (dibSize < 40 || width <= 0 || height == 0 || planes != 1 ||
      (bitsPerPixel != 8 && bitsPerPixel != 24 && bitsPerPixel != 32) ||
      compression != 0 || pixelOffset >= data.size()) {
    error = "unsupported BMP encoding: " + path.string();
    return false;
  }

  const bool bottomUp = height > 0;
  const int imageHeight = height > 0 ? height : -height;
  const std::size_t rowStride =
      ((static_cast<std::size_t>(width) * bitsPerPixel + 31) / 32) * 4;
  const std::size_t paletteEntries = bitsPerPixel == 8
                                         ? (read32(data.data() + 46) != 0
                                                ? read32(data.data() + 46)
                                                : 256)
                                         : 0;
  const std::size_t paletteOffset = 14 + dibSize;
  if (paletteOffset + paletteEntries * 4 > data.size() ||
      pixelOffset + rowStride * static_cast<std::size_t>(imageHeight) >
          data.size()) {
    error = "truncated BMP data: " + path.string();
    return false;
  }

  ImageRgba loaded;
  loaded.width = width;
  loaded.height = imageHeight;
  loaded.pixels.resize(static_cast<std::size_t>(width) * imageHeight * 4);
  for (int y = 0; y < imageHeight; ++y) {
    const int sourceY = bottomUp ? imageHeight - 1 - y : y;
    const std::uint8_t* row = data.data() + pixelOffset +
                              rowStride * static_cast<std::size_t>(sourceY);
    for (int x = 0; x < width; ++x) {
      std::uint8_t red = 0;
      std::uint8_t green = 0;
      std::uint8_t blue = 0;
      std::uint8_t alpha = 255;
      if (bitsPerPixel == 8) {
        const std::uint8_t index = row[x];
        const std::uint8_t* color = data.data() + paletteOffset + index * 4;
        blue = color[0];
        green = color[1];
        red = color[2];
        if (index == 0)
          alpha = 0;
      } else if (bitsPerPixel == 24) {
        const std::uint8_t* color = row + x * 3;
        blue = color[0];
        green = color[1];
        red = color[2];
      } else {
        const std::uint8_t* color = row + x * 4;
        blue = color[0];
        green = color[1];
        red = color[2];
        alpha = color[3];
      }

      const std::size_t destination =
          (static_cast<std::size_t>(y) * width + x) * 4;
      loaded.pixels[destination + 0] = red;
      loaded.pixels[destination + 1] = green;
      loaded.pixels[destination + 2] = blue;
      loaded.pixels[destination + 3] = alpha;
    }
  }

  image = std::move(loaded);
  return true;
}

} // namespace geocube::render
