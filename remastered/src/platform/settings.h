#ifndef GEOCUBE_PLATFORM_SETTINGS_H
#define GEOCUBE_PLATFORM_SETTINGS_H

#include "core/game_world.h"
#include "core/high_scores.h"

#include <array>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

namespace geocube::platform {

using ControlBindings =
    std::array<std::string, core::actionIndex(core::Action::Count)>;

ControlBindings defaultControlBindings();
const char* controlActionName(core::Action action);

struct Settings {
  bool music = true;
  bool effects = true;
  bool fullscreen = false;
  int width = 960;
  int height = 720;
  std::string playerName = "Pilot";
  std::vector<core::HighScoreEntry> highScores;
  ControlBindings controls;

  Settings();
};

class SettingsStore final {
public:
  SettingsStore() = default;
  explicit SettingsStore(std::filesystem::path directory)
      : m_directory(std::move(directory))
  {
  }

  bool load(Settings& settings);
  bool save(const Settings& settings) const;
  const std::filesystem::path& directory() const { return m_directory; }

private:
  bool findDirectory();
  std::filesystem::path m_directory;
};

} // namespace geocube::platform

#endif
