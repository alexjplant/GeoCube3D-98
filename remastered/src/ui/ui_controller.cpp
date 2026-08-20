#include "ui/ui_controller.h"

#include "core/high_scores.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <utility>

namespace geocube::ui {
namespace {

constexpr core::Vec3 kPanel{0.02f, 0.03f, 0.10f};
constexpr core::Vec3 kText{0.95f, 0.90f, 0.25f};
constexpr core::Vec3 kHighlight{1.0f, 0.25f, 0.15f};
constexpr core::Vec3 kThrust{1.0f, 0.65f, 0.10f};
constexpr core::Vec3 kShield{0.10f, 0.80f, 1.0f};
constexpr core::Vec3 kShieldTrack{0.06f, 0.10f, 0.18f};
constexpr std::array<core::Action, 17> kConfigurableActions{{
    core::Action::AimUp, core::Action::AimDown, core::Action::AimLeft,
    core::Action::AimRight, core::Action::ThrustForward,
    core::Action::ThrustBackward, core::Action::ThrustLeft,
    core::Action::ThrustRight, core::Action::Fire, core::Action::Shield,
    core::Action::Pause, core::Action::FullStop, core::Action::HighScores,
    core::Action::Quit, core::Action::ZoomIn,
    core::Action::ZoomOut, core::Action::Confirm}};

std::string formatLevelTime(double seconds)
{
  const int totalSeconds = std::max(0, static_cast<int>(seconds));
  const int minutes = totalSeconds / 60;
  const int remainingSeconds = totalSeconds % 60;
  return "TIME " + std::to_string(minutes) + ":" +
         (remainingSeconds < 10 ? "0" : "") +
         std::to_string(remainingSeconds);
}

} // namespace

bool UiController::initialize(platform::Settings settings)
{
  m_settings = std::move(settings);
  m_screen = Screen::MainMenu;
  m_menuSelection = 0;
  m_optionSelection = 0;
  m_controlSelection = 0;
  m_selectedLevel = 0;
  m_pauseSelection = 0;
  m_waitingForBinding = false;
  m_scoreInserted = false;
  return true;
}

bool UiController::anyConfirm(const core::InputState& input) const
{
  return input.wasPressed(core::Action::Confirm) ||
         input.wasPressed(core::Action::Pause);
}

UiCommand UiController::handleInput(const core::InputState& input,
                                     const core::GameWorld& world,
                                     std::string_view textInput,
                                     std::string_view capturedKey)
{
  UiCommand command;
  if (input.wasPressed(core::Action::Quit)) {
    if (m_screen == Screen::Playing) {
      m_previousScreen = Screen::Playing;
      m_pauseSelection = 0;
      m_screen = Screen::Paused;
      command.pauseMenuOpened = true;
    } else if (m_screen == Screen::Paused) {
      m_screen = m_previousScreen;
      command.resumeGame = true;
    } else if (m_screen == Screen::Controls) {
      const bool fromPause = m_previousScreen == Screen::Paused;
      m_screen = m_previousScreen;
      command.pauseMenuOpened = fromPause;
    } else if (m_screen == Screen::NameEntry ||
                m_screen == Screen::Options || m_screen == Screen::HighScores ||
                m_screen == Screen::About ||
               m_screen == Screen::GameOver) {
      m_screen = m_previousScreen;
    } else {
      command.quit = true;
    }
    return command;
  }
  const bool up = input.wasPressed(core::Action::AimUp);
  const bool down = input.wasPressed(core::Action::AimDown);
  if (m_screen == Screen::MainMenu) {
    if (up)
      m_menuSelection = (m_menuSelection + 4) % 5;
    if (down)
      m_menuSelection = (m_menuSelection + 1) % 5;
    if (anyConfirm(input)) {
      switch (m_menuSelection) {
      case 0:
        m_previousScreen = Screen::MainMenu;
        m_screen = Screen::NameEntry;
        command.previewLevel = m_selectedLevel;
        break;
      case 1:
        m_previousScreen = Screen::MainMenu;
        m_screen = Screen::Options;
        break;
      case 2:
        m_previousScreen = Screen::MainMenu;
        m_screen = Screen::Controls;
        break;
      case 3:
        m_previousScreen = Screen::MainMenu;
        m_screen = Screen::About;
        break;
      case 4:
        command.quit = true;
        break;
      }
    }
    return command;
  }

  if (m_screen == Screen::Controls) {
    if (!capturedKey.empty() && m_waitingForBinding) {
      const core::Action action = kConfigurableActions[
          static_cast<std::size_t>(m_controlSelection)];
      m_settings.controls[core::actionIndex(action)] = capturedKey;
      m_waitingForBinding = false;
      command.saveSettings = true;
    }
    if (up)
      m_controlSelection =
          (m_controlSelection + static_cast<int>(kConfigurableActions.size()) -
           1) % static_cast<int>(kConfigurableActions.size());
    if (down)
      m_controlSelection =
          (m_controlSelection + 1) % static_cast<int>(kConfigurableActions.size());
    if (anyConfirm(input) && !m_waitingForBinding) {
      m_waitingForBinding = true;
      command.captureControl = true;
    }
    return command;
  }

  if (m_screen == Screen::NameEntry) {
    if (up)
      m_selectedLevel = (m_selectedLevel + 4) % 5;
    if (down)
      m_selectedLevel = (m_selectedLevel + 1) % 5;
    if (up || down)
      command.previewLevel = m_selectedLevel;
    for (char character : textInput) {
      if (m_settings.playerName.size() < 16 && character >= 32 &&
          character <= 126)
        m_settings.playerName.push_back(character);
    }
    if (input.wasPressed(core::Action::Back) &&
        !m_settings.playerName.empty())
      m_settings.playerName.pop_back();
    if (anyConfirm(input)) {
      if (m_settings.playerName.empty())
        m_settings.playerName = "Pilot";
      command.startGame = true;
      command.startLevel = m_selectedLevel;
      m_screen = Screen::Playing;
      m_scoreInserted = false;
    }
    return command;
  }

  if (m_screen == Screen::Playing) {
    if (input.wasPressed(core::Action::Pause)) {
      m_previousScreen = Screen::Playing;
      m_pauseSelection = 0;
      m_screen = Screen::Paused;
    } else if (input.wasPressed(core::Action::HighScores)) {
      m_previousScreen = Screen::Playing;
      m_screen = Screen::HighScores;
    }
    if (world.state() == core::GameState::GameOver)
    {
      m_previousScreen = Screen::MainMenu;
      m_screen = Screen::GameOver;
    }
    return command;
  }

  if (m_screen == Screen::Paused) {
    if (up)
      m_pauseSelection = (m_pauseSelection + 2) % 3;
    if (down)
      m_pauseSelection = (m_pauseSelection + 1) % 3;
    if (anyConfirm(input)) {
      if (m_pauseSelection == 0) {
        m_screen = m_previousScreen;
      } else if (m_pauseSelection == 1) {
        command.endGame = true;
        m_screen = Screen::MainMenu;
      } else {
        m_previousScreen = Screen::Paused;
        m_controlSelection = 0;
        m_screen = Screen::Controls;
      }
    }
    return command;
  }

  if (m_screen == Screen::Options) {
    if (up)
      m_optionSelection = (m_optionSelection + 2) % 3;
    if (down)
      m_optionSelection = (m_optionSelection + 1) % 3;
    if (input.wasPressed(core::Action::AimLeft) ||
        input.wasPressed(core::Action::AimRight)) {
      if (m_optionSelection == 0)
        m_settings.music = !m_settings.music;
      else if (m_optionSelection == 1)
        m_settings.effects = !m_settings.effects;
      else
        m_settings.fullscreen = !m_settings.fullscreen;
      command.saveSettings = true;
      command.fullscreenChanged = m_optionSelection == 2;
    }
    if (anyConfirm(input)) {
      m_screen = m_previousScreen;
      command.saveSettings = true;
    }
    return command;
  }

  if (m_screen == Screen::GameOver && !m_scoreInserted) {
    insertHighScore(m_settings.highScores,
                    {m_settings.playerName, world.score(), world.levelNumber()});
    m_scoreInserted = true;
    command.saveSettings = true;
  }

  if (anyConfirm(input))
    m_screen = m_previousScreen;
  return command;
}

void UiController::drawMenu(render::Renderer& renderer) const
{
  const float sx = renderer.width() / 960.0f;
  const float sy = renderer.height() / 720.0f;
  renderer.drawUiRect(0.0f, 0.0f, renderer.width(), renderer.height(), kPanel);
  renderer.drawUiText("GEOCUBE 3D", 220.0f * sx, 90.0f * sy, 8.0f * sy,
                      kText);
  const char* entries[] = {"NEW GAME", "OPTIONS", "CONTROLS", "ABOUT",
                           "QUIT"};
  for (int i = 0; i < 5; ++i) {
    const std::string prefix = i == m_menuSelection ? "> " : "  ";
    renderer.drawUiText(prefix + entries[i], 300.0f * sx,
                        (250.0f + i * 55.0f) * sy, 4.0f * sy,
                        i == m_menuSelection ? kHighlight : kText);
  }
  renderer.drawUiText("ENTER SELECT   UP DOWN MOVE", 250.0f * sx, 620.0f * sy,
                      2.0f * sy,
                      kText);
}

void UiController::drawOverlay(render::Renderer& renderer,
                                const std::string& title,
                                const std::string& body) const
{
  const float sx = renderer.width() / 960.0f;
  const float sy = renderer.height() / 720.0f;
  renderer.drawUiRect(80.0f * sx, 100.0f * sy, 800.0f * sx, 520.0f * sy,
                      kPanel);
  renderer.drawUiText(title, 130.0f * sx, 145.0f * sy, 5.0f * sy, kText);
  renderer.drawUiText(body, 130.0f * sx, 240.0f * sy, 3.0f * sy, kText);
  renderer.drawUiText("ENTER OR BACK TO RETURN", 130.0f * sx, 560.0f * sy,
                      2.0f * sy,
                      kText);
}

void UiController::draw(render::Renderer& renderer,
                         const core::GameWorld& world) const
{
  if (m_screen == Screen::MainMenu) {
    drawMenu(renderer);
    return;
  }

  if (m_screen == Screen::Playing) {
    const float sx = renderer.width() / 960.0f;
    const float sy = renderer.height() / 720.0f;
    renderer.drawUiRect(0.0f, 0.0f, renderer.width(), 78.0f * sy, kPanel);
    renderer.drawUiText("LEVEL " + std::to_string(world.levelNumber()),
                        20.0f * sx, 12.0f * sy, 3.0f * sy, kText);
    renderer.drawUiText(formatLevelTime(world.levelElapsedSeconds()),
                        160.0f * sx, 12.0f * sy, 3.0f * sy, kText);
    renderer.drawUiText("LIVES " + std::to_string(world.lives()), 760.0f * sx,
                        12.0f * sy, 3.0f * sy, kText);
    renderer.drawUiText("THRUST", 535.0f * sx, 13.0f * sy, 2.0f * sy, kText);
    const float thrustFuel =
        std::clamp(world.thrustFuelFraction(), 0.0f, 1.0f);
    renderer.drawUiRect(635.0f * sx, 10.0f * sy, 100.0f * sx, 16.0f * sy,
                        kShieldTrack);
    renderer.drawUiRect(637.0f * sx, 12.0f * sy,
                        96.0f * sx * thrustFuel, 12.0f * sy, kThrust);
    renderer.drawUiText("SHIELD", 535.0f * sx, 44.0f * sy, 2.0f * sy, kText);
    const float shieldCharge =
        std::clamp(world.shieldChargeFraction(), 0.0f, 1.0f);
    renderer.drawUiRect(635.0f * sx, 41.0f * sy, 100.0f * sx, 16.0f * sy,
                        kShieldTrack);
    renderer.drawUiRect(637.0f * sx, 43.0f * sy,
                        96.0f * sx * shieldCharge, 12.0f * sy, kShield);
    if (world.state() == core::GameState::PlayerHit) {
      const int seconds = static_cast<int>(std::ceil(
          world.playerHitRemainingSeconds()));
      drawOverlay(renderer, "SHIP HIT",
                  "RESPAWNING IN " + std::to_string(seconds));
    }
    return;
  }

  if (m_screen == Screen::NameEntry) {
    drawOverlay(renderer, "NEW GAME",
                "PLAYER NAME:\n" + m_settings.playerName +
                    "\n> START LEVEL " + std::to_string(m_selectedLevel + 1) +
                    "\nUP/DOWN SELECT LEVEL\nTYPE NAME THEN PRESS ENTER");
    return;
  }

  if (m_screen == Screen::Controls) {
    const float sx = renderer.width() / 960.0f;
    const float sy = renderer.height() / 720.0f;
    renderer.drawUiRect(80.0f * sx, 70.0f * sy, 800.0f * sx, 580.0f * sy,
                        kPanel);
    renderer.drawUiText("CONTROLS", 130.0f * sx, 105.0f * sy, 5.0f * sy,
                        kText);
    for (std::size_t i = 0; i < kConfigurableActions.size(); ++i) {
      const core::Action action = kConfigurableActions[i];
      const int column = static_cast<int>(i / 9);
      const int row = static_cast<int>(i % 9);
      const float x = (110.0f + column * 400.0f) * sx;
      const float y = (185.0f + row * 38.0f) * sy;
      std::string line = static_cast<int>(i) == m_controlSelection ? "> " : "  ";
      line += platform::controlActionName(action) + std::string(": ");
      line += m_settings.controls[core::actionIndex(action)];
      renderer.drawUiText(line, x, y, 2.2f * sy,
                          static_cast<int>(i) == m_controlSelection
                              ? kHighlight
                              : kText);
    }
    renderer.drawUiText(m_waitingForBinding ? "PRESS A KEY TO BIND"
                                           : "ENTER REBIND   ESCAPE BACK",
                        130.0f * sx, 600.0f * sy, 2.2f * sy, kText);
    return;
  }

  if (m_screen == Screen::Paused) {
    const char* options[] = {"RESUME", "END GAME", "CONTROLS"};
    std::string body;
    for (int i = 0; i < 3; ++i) {
      body += i == m_pauseSelection ? "> " : "  ";
      body += options[i];
      body += "\n";
    }
    drawOverlay(renderer, "PAUSED", body);
  } else if (m_screen == Screen::HighScores) {
    std::string body;
    for (std::size_t i = 0; i < m_settings.highScores.size() && i < 10; ++i)
      body += std::to_string(i + 1) + " " +
              m_settings.highScores[i].playerName + " " +
              std::to_string(m_settings.highScores[i].score) + "\n";
    drawOverlay(renderer, "HIGH SCORES", body);
  } else if (m_screen == Screen::Options) {
    const std::array<std::string, 3> options{
        std::string("MUSIC ") + (m_settings.music ? "ON" : "OFF"),
        std::string("EFFECTS ") + (m_settings.effects ? "ON" : "OFF"),
        std::string("FULLSCREEN ") +
            (m_settings.fullscreen ? "ON" : "OFF")};
    std::string body;
    for (std::size_t i = 0; i < options.size(); ++i)
      body += (static_cast<int>(i) == m_optionSelection ? "> " : "  ") +
              options[i] + "\n";
    drawOverlay(renderer, "OPTIONS", body +
                                      "LEFT/RIGHT CHANGE\nENTER RETURN");
  } else if (m_screen == Screen::About) {
    drawOverlay(renderer, "ABOUT",
                "GEOCUBE 3D REMASTERED\nAUTHOR ANDY WESTON\n"
                "GITHUB @WESTONSOFTWARE\nREMASTERED BY ALEX PLANT\n"
                "GITHUB @ALEXJPLANT\nSDL3 OPENGL 3.3");
  } else if (m_screen == Screen::GameOver) {
    drawOverlay(renderer, "GAME OVER",
                "SCORE " + std::to_string(world.score()) + "\nLEVEL " +
                    std::to_string(world.levelNumber()));
  }
}

} // namespace geocube::ui
