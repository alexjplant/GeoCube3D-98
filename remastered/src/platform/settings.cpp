#include "platform/settings.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <exception>
#include <fstream>

namespace geocube::platform {

ControlBindings defaultControlBindings()
{
  ControlBindings bindings{};
  const auto set = [&bindings](core::Action action, const char* key) {
    bindings[core::actionIndex(action)] = key;
  };
  set(core::Action::AimUp, "Up");
  set(core::Action::AimDown, "Down");
  set(core::Action::AimLeft, "Left");
  set(core::Action::AimRight, "Right");
  set(core::Action::ThrustForward, "A");
  set(core::Action::ThrustBackward, "Z");
  set(core::Action::ThrustLeft, "Comma");
  set(core::Action::ThrustRight, "Period");
  set(core::Action::Fire, "Space");
  set(core::Action::Shield, "S");
  set(core::Action::Pause, "Pause");
  set(core::Action::FullStop, "C");
  set(core::Action::HighScores, "H");
  set(core::Action::Quit, "Escape");
  set(core::Action::ZoomIn, "Page Up");
  set(core::Action::ZoomOut, "Page Down");
  set(core::Action::Confirm, "Return");
  // Back navigation is always Escape. Backspace is reserved for text editing.
  return bindings;
}

const char* controlActionName(core::Action action)
{
  switch (action) {
  case core::Action::AimUp: return "Aim Up";
  case core::Action::AimDown: return "Aim Down";
  case core::Action::AimLeft: return "Aim Left";
  case core::Action::AimRight: return "Aim Right";
  case core::Action::ThrustForward: return "Thrust Forward";
  case core::Action::ThrustBackward: return "Thrust Backward";
  case core::Action::ThrustLeft: return "Thrust Left";
  case core::Action::ThrustRight: return "Thrust Right";
  case core::Action::Fire: return "Fire";
  case core::Action::Shield: return "Shield";
  case core::Action::Pause: return "Pause";
  case core::Action::FullStop: return "Full Stop";
  case core::Action::HighScores: return "High Scores";
  case core::Action::Quit: return "Quit";
  case core::Action::ZoomIn: return "Zoom In";
  case core::Action::ZoomOut: return "Zoom Out";
  case core::Action::Confirm: return "Confirm";
  case core::Action::Back: return "Back";
  case core::Action::Count: return "";
  }
  return "";
}

Settings::Settings() : controls(defaultControlBindings())
{
}

bool SettingsStore::findDirectory()
{
  char* preferencePath = SDL_GetPrefPath("WestonSoftware", "GeoCube3D");
  if (!preferencePath)
    return false;
  m_directory = preferencePath;
  SDL_free(preferencePath);
  std::error_code error;
  std::filesystem::create_directories(m_directory, error);
  return !error;
}

bool SettingsStore::load(Settings& settings)
{
  if (m_directory.empty() && !findDirectory())
    return false;

  std::ifstream input(m_directory / "settings.ini");
  const auto parseDimension = [](const std::string& value,
                                 int fallback) {
    try {
      return std::max(1, std::stoi(value));
    } catch (const std::exception&) {
      return fallback;
    }
  };
  std::string line;
  while (std::getline(input, line)) {
    const std::size_t separator = line.find('=');
    if (separator == std::string::npos)
      continue;
    const std::string key = line.substr(0, separator);
    const std::string value = line.substr(separator + 1);
    if (key == "music")
      settings.music = value == "1";
    else if (key == "effects")
      settings.effects = value == "1";
    else if (key == "fullscreen")
      settings.fullscreen = value == "1";
    else if (key == "width")
      settings.width = std::max(320, parseDimension(value, settings.width));
    else if (key == "height")
      settings.height = std::max(240, parseDimension(value, settings.height));
    else if (key == "player_name")
      settings.playerName = value;
    else if (key.rfind("control.", 0) == 0) {
      const std::string actionName = key.substr(8);
      for (std::size_t i = 0; i < core::actionIndex(core::Action::Count);
           ++i) {
        const auto action = static_cast<core::Action>(i);
        if (actionName == controlActionName(action))
          settings.controls[i] = value;
      }
    }
  }

  std::ifstream scores(m_directory / "highscores.txt");
  while (std::getline(scores, line)) {
    const std::size_t first = line.find('|');
    const std::size_t second = line.find('|', first + 1);
    if (first == std::string::npos || second == std::string::npos)
      continue;
    try {
      settings.highScores.push_back({line.substr(second + 1),
                                     std::stoi(line.substr(0, first)),
                                     std::stoi(line.substr(first + 1,
                                                            second - first - 1))});
    } catch (const std::exception&) {
      continue;
    }
  }
  return true;
}

bool SettingsStore::save(const Settings& settings) const
{
  if (m_directory.empty())
    return false;

  std::ofstream output(m_directory / "settings.ini");
  if (!output)
    return false;
  output << "music=" << (settings.music ? 1 : 0) << '\n'
         << "effects=" << (settings.effects ? 1 : 0) << '\n'
         << "fullscreen=" << (settings.fullscreen ? 1 : 0) << '\n'
         << "width=" << settings.width << '\n'
         << "height=" << settings.height << '\n'
         << "player_name=" << settings.playerName << '\n';
  for (std::size_t i = 0; i < core::actionIndex(core::Action::Count); ++i)
    output << "control." << controlActionName(static_cast<core::Action>(i))
           << '=' << settings.controls[i] << '\n';

  std::ofstream scores(m_directory / "highscores.txt");
  if (!scores)
    return false;
  for (const core::HighScoreEntry& score : settings.highScores)
    scores << score.score << '|' << score.level << '|' << score.playerName
           << '\n';
  return true;
}

} // namespace geocube::platform
