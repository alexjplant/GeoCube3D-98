#ifndef GEOCUBE_UI_CONTROLLER_H
#define GEOCUBE_UI_CONTROLLER_H

#include "core/game_world.h"
#include "platform/settings.h"
#include "render/renderer.h"

#include "core/math.h"

#include <string_view>

namespace geocube::ui {

enum class Screen {
  MainMenu,
  NameEntry,
  Controls,
  Playing,
  Paused,
  HighScores,
  Options,
  About,
  GameOver,
};

struct UiCommand {
  bool quit = false;
  bool startGame = false;
  int startLevel = 0;
  int previewLevel = -1;
  bool saveSettings = false;
  bool fullscreenChanged = false;
  bool captureControl = false;
  bool pauseMenuOpened = false;
  bool resumeGame = false;
  bool endGame = false;
};

class UiController final {
public:
  bool initialize(platform::Settings settings);
  UiCommand handleInput(const core::InputState& input,
                        const core::GameWorld& world,
                        std::string_view textInput = {},
                        std::string_view capturedKey = {});
  void draw(render::Renderer& renderer, const core::GameWorld& world) const;

  Screen screen() const { return m_screen; }
  const platform::Settings& settings() const { return m_settings; }
  void setScreen(Screen screen) { m_screen = screen; }

private:
  void drawMenu(render::Renderer& renderer) const;
  void drawGameplayIndicators(render::Renderer& renderer,
                               const core::GameWorld& world) const;
  void drawOverlay(render::Renderer& renderer, const std::string& title,
                   const std::string& body) const;
  bool anyConfirm(const core::InputState& input) const;

  platform::Settings m_settings;
  Screen m_screen = Screen::MainMenu;
  Screen m_previousScreen = Screen::MainMenu;
  int m_menuSelection = 0;
  int m_optionSelection = 0;
  int m_controlSelection = 0;
  int m_selectedLevel = 0;
  int m_pauseSelection = 0;
  bool m_waitingForBinding = false;
  bool m_scoreInserted = false;
  mutable core::Vec3 m_displayVelocity;
  mutable core::Vec3 m_displayThrust;
  mutable float m_displayVelocityRate = 0.0f;
  mutable float m_displayThrustRate = 0.0f;
};

} // namespace geocube::ui

#endif
