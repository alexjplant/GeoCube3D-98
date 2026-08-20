#include <array>
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
  constexpr std::array meshes{"models/ship.mesh", "models/cube.mesh"};
  constexpr std::array convertedModels{"ship", "cube", "jack", "portal",
                                       "sphere3", "pasta"};
  constexpr std::array bitmaps{"textures/bullet.bmp", "textures/canvas.bmp",
                               "textures/explode.bmp", "textures/logo.bmp"};
  constexpr std::array waves{"audio/effects/fire.wav",
                             "audio/effects/laser.wav",
                             "audio/effects/thrust.wav",
                             "audio/effects/shield.wav"};
  constexpr std::array midi{"audio/music/music.mid", "audio/music/synth.mid",
                            "audio/music/reggae.mid", "audio/music/spin.mid",
                            "audio/music/funk.mid"};
  constexpr const char* soundFont = "audio/music/Roland.SC-55.sf2";

  bool valid = true;
  for (const char* relative : meshes) {
    if (!isRegular(root / relative) || !isRemasteredMesh(root / relative)) {
      std::cerr << "Invalid mesh: " << relative << '\n';
      valid = false;
    }
  }
  for (const char* name : convertedModels) {
    const std::filesystem::path obj =
        root / "models" / (std::string(name) + ".obj");
    const std::filesystem::path mtl =
        root / "models" / (std::string(name) + ".mtl");
    if (!isRegular(obj) || !isRegular(mtl)) {
      std::cerr << "Missing converted model pair: " << name << '\n';
      valid = false;
    }
  }
  for (const char* relative : bitmaps) {
    if (!isRegular(root / relative) || !isBmp(root / relative)) {
      std::cerr << "Invalid BMP: " << relative << '\n';
      valid = false;
    }
  }
  for (const char* relative : waves) {
    if (!isRegular(root / relative) || !hasPrefix(root / relative, "RIFF")) {
      std::cerr << "Invalid WAV: " << relative << '\n';
      valid = false;
    }
  }
  for (const char* relative : midi) {
    if (!isRegular(root / relative) || !hasPrefix(root / relative, "MThd")) {
      std::cerr << "Invalid MIDI inventory asset: " << relative << '\n';
      valid = false;
    }
  }
  if (!isRegular(root / soundFont) || !isSoundFont(root / soundFont)) {
    std::cerr << "Invalid SoundFont: " << soundFont << '\n';
    valid = false;
  }

  if (valid)
    std::cout << "Remastered asset validation passed\n";
  return valid ? 0 : 1;
}
