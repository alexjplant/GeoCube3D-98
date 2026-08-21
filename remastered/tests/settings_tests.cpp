#include "platform/settings.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace {

void expect(bool condition, const char* message)
{
  if (!condition) {
    std::cerr << message << '\n';
    std::exit(EXIT_FAILURE);
  }
}

} // namespace

int main()
{
  const geocube::platform::Settings defaults;
  const auto& controls = defaults.controls;
  expect(controls[geocube::core::actionIndex(geocube::core::Action::ThrustForward)] == "W",
         "forward thrust default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::ThrustBackward)] == "S",
         "backward thrust default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::ThrustLeft)] == "A",
         "left thrust default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::ThrustRight)] == "D",
         "right thrust default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::Shield)] == "Left Shift",
         "shield default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::FullStop)] == "C",
         "full stop default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::RollLeft)] == "Q",
         "roll left default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::RollRight)] == "E",
         "roll right default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::ZoomIn)] == "R",
         "zoom in default");
  expect(controls[geocube::core::actionIndex(geocube::core::Action::ZoomOut)] == "F",
         "zoom out default");

  const std::filesystem::path directory =
      std::filesystem::temp_directory_path() / "geocube3d-settings-test";
  std::error_code error;
  std::filesystem::remove_all(directory, error);
  std::filesystem::create_directories(directory, error);

  geocube::platform::Settings saved;
  saved.music = false;
  saved.effects = false;
  saved.fullscreen = true;
  saved.width = 1280;
  saved.height = 800;
  saved.playerName = "Test Pilot";
  saved.controls[geocube::core::actionIndex(geocube::core::Action::Fire)] =
      "Numpad 5";
  saved.highScores.push_back({"A|B", 100, 2});

  geocube::platform::SettingsStore store(directory);
  expect(store.save(saved), "settings save succeeds");

  geocube::platform::Settings loaded;
  expect(store.load(loaded), "settings load succeeds");
  expect(!loaded.music && !loaded.effects && loaded.fullscreen,
         "boolean settings round trip");
  expect(loaded.width == 1280 && loaded.height == 800,
         "dimensions round trip");
  expect(loaded.playerName == "Test Pilot", "player name round trip");
  expect(loaded.controls[geocube::core::actionIndex(geocube::core::Action::Fire)] ==
             "Numpad 5",
         "control binding round trip");
  expect(loaded.highScores.size() == 1 && loaded.highScores[0].playerName == "A|B",
         "legacy delimiter behavior is preserved");

  loaded.highScores.push_back({"Duplicate", 1, 1});
  expect(store.load(loaded), "repeated settings load succeeds");
  expect(loaded.highScores.size() == 3,
         "repeated load preserves the legacy duplicate behavior");

  std::filesystem::remove_all(directory, error);
  std::cout << "Settings characterization tests passed\n";
}
