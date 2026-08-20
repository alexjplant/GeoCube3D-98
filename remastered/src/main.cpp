#include "core/game_world.h"
#include "audio/audio_system.h"
#include "input/sdl_input.h"
#include "platform/frame_clock.h"
#include "platform/resource_root.h"
#include "platform/settings.h"
#include "platform/sdl_shell.h"
#include "render/renderer.h"
#include "ui/ui_controller.h"

#include <SDL3/SDL.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

namespace {

struct Options {
  bool headless = false;
  bool demo = false;
  int frameLimit = -1;
};

void printUsage(const char* program)
{
  std::cout << "Usage: " << program << " [--headless] [--frames N]\n"
            << "  --headless   resolve and validate assets without opening a window\n"
            << "  --demo       start directly in the playable world for smoke tests\n"
            << "  --frames N   exit after rendering N frames\n";
}

bool parseOptions(int argc, char** argv, Options& options)
{
  for (int i = 1; i < argc; ++i) {
    const std::string argument(argv[i]);
    if (argument == "--headless") {
      options.headless = true;
    } else if (argument == "--demo") {
      options.demo = true;
    } else if (argument == "--frames" && i + 1 < argc) {
      char* end = nullptr;
      const long value = std::strtol(argv[++i], &end, 10);
      if (*end != '\0' || value < 1 || value > 1000000) {
        std::cerr << "--frames requires an integer from 1 to 1000000\n";
        return false;
      }
      options.frameLimit = static_cast<int>(value);
    } else if (argument == "--help" || argument == "-h") {
      printUsage(argv[0]);
      return false;
    } else {
      std::cerr << "Unknown option: " << argument << '\n';
      printUsage(argv[0]);
      return false;
    }
  }
  return true;
}

bool validateRemasteredAssets(const std::filesystem::path& root)
{
  constexpr std::array requiredAssets{
      "models/ship.mesh",
      "models/cube.mesh",
      "textures/canvas.bmp",
      "textures/bullet.bmp",
      "audio/effects/fire.wav",
#if defined(__EMSCRIPTEN__)
      "audio/music/music.ogg",
      "audio/music/synth.ogg",
      "audio/music/reggae.ogg",
      "audio/music/spin.ogg",
      "audio/music/funk.ogg",
      "audio/music/snare.ogg",
      "audio/music/wierd.ogg",
#else
      "audio/music/music.mid",
      "audio/music/Roland.SC-55.sf2",
#endif
  };

  bool valid = true;
  for (const char* asset : requiredAssets) {
    if (!geocube::platform::hasAsset(root, asset)) {
      std::cerr << "Missing remastered asset: " << asset << '\n';
      valid = false;
    }
  }
  return valid;
}

int runHeadless()
{
  if (!SDL_Init(0)) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
    return 1;
  }

  const std::filesystem::path root =
      geocube::platform::discoverResourceRoot();
  if (root.empty()) {
    std::cerr << "Remastered asset root was not found relative to the executable\n";
    SDL_Quit();
    return 2;
  }

  const bool valid = validateRemasteredAssets(root);
  std::cout << "Remastered asset root: " << root << '\n';
  SDL_Quit();
  return valid ? 0 : 2;
}

} // namespace

class Application final {
public:
  explicit Application(Options options) : m_options(std::move(options)) {}

  bool initialize()
  {
    if (!m_shell.initialize("GeoCube 3D Remastered", 960, 720))
      return false;

    m_root = geocube::platform::discoverResourceRoot();
    if (m_root.empty() || !validateRemasteredAssets(m_root)) {
      std::cerr << "Remastered assets are unavailable; refusing to start\n";
      return false;
    }

    if (!m_renderer.initialize(geocube::platform::SdlShell::glProcAddress,
                               m_root)) {
      std::cerr << "Renderer initialization failed: " << m_renderer.error()
                << '\n';
      return false;
    }
    m_settingsStore.load(m_settings);
#ifndef __EMSCRIPTEN__
    m_shell.setFullscreen(m_settings.fullscreen);
#endif
    m_ui.initialize(m_settings);
    m_input.setBindings(m_ui.settings().controls);

    if (!m_audio.initialize(m_root))
      std::cerr << "Audio disabled: " << m_audio.error() << '\n';
    else if (!m_audio.error().empty())
      std::cerr << "Audio warning: " << m_audio.error() << '\n';

    if (m_options.demo) {
      m_world.startNewGame(m_ui.settings().playerName);
      m_ui.setScreen(geocube::ui::Screen::Playing);
      if (m_ui.settings().music) {
        m_audio.playMusic(m_world.currentLevel().music);
        m_activeMusic = m_world.currentLevel().music;
      }
    }

    int windowWidth = 1;
    int windowHeight = 1;
    m_shell.windowSizeInPixels(windowWidth, windowHeight);
    m_renderer.resize(windowWidth, windowHeight);
    std::cout << "GeoCube 3D Remastered SDL shell\n"
              << "Resource root: " << m_root << '\n'
              << "OpenGL: " << m_shell.glVersion() << '\n'
              << "F11 toggles fullscreen; Escape opens the pause menu\n";
    return true;
  }

  bool frame()
  {
    m_input.beginFrame();
    const geocube::platform::ShellEvents events = m_shell.processEvents(
        geocube::input::SdlInput::eventCallback, &m_input);
    m_quit = events.quit;

    if (events.focusChanged) {
      m_clock.reset();
      std::cout << (events.focused ? "Window focus gained\n"
                                   : "Window focus lost\n");
    }

    if (!m_shell.focused()) {
#ifndef __EMSCRIPTEN__
      SDL_Delay(50);
#endif
      return !m_quit;
    }

    const double deltaSeconds = m_clock.tick();
    int windowWidth = 1;
    int windowHeight = 1;
    if (events.resized) {
      m_shell.windowSizeInPixels(windowWidth, windowHeight);
      m_renderer.resize(windowWidth, windowHeight);
    }
    const geocube::core::InputState& inputState = m_input.state();
    const bool wasPlaying =
        m_ui.screen() == geocube::ui::Screen::Playing;
    const geocube::ui::UiCommand command =
        m_ui.handleInput(inputState, m_world, m_input.textInput(),
                         m_input.capturedKeyName());
    m_shell.setTextInput(m_ui.screen() == geocube::ui::Screen::NameEntry);
    m_input.setBindings(m_ui.settings().controls);
    if (command.captureControl)
      m_input.startKeyCapture();
    if (command.previewLevel >= 0)
      m_world.previewLevel(command.previewLevel);
    if (command.endGame) {
      m_world.startNewGame(m_ui.settings().playerName, command.startLevel);
      m_audio.stopMusic();
      m_activeMusic.clear();
    }
    if (command.quit)
      m_quit = true;
    if (command.startGame) {
      m_world.startNewGame(m_ui.settings().playerName, command.startLevel);
      if (m_ui.settings().music) {
        m_audio.playMusic(m_world.currentLevel().music);
        m_activeMusic = m_world.currentLevel().music;
      }
    }
    if (command.saveSettings)
      m_settingsStore.save(m_ui.settings());
    if (command.fullscreenChanged) {
      m_shell.setFullscreen(m_ui.settings().fullscreen);
      m_shell.windowSizeInPixels(windowWidth, windowHeight);
      m_renderer.resize(windowWidth, windowHeight);
    }

    if (m_ui.settings().effects &&
        inputState.wasPressed(geocube::core::Action::Fire))
      m_audio.playEffect(geocube::audio::Effect::Fire);
    const bool thrusting =
        inputState.isHeld(geocube::core::Action::ThrustForward) ||
        inputState.isHeld(geocube::core::Action::ThrustBackward) ||
        inputState.isHeld(geocube::core::Action::ThrustLeft) ||
        inputState.isHeld(geocube::core::Action::ThrustRight);
    if (m_ui.settings().effects && thrusting)
      m_audio.playEffect(geocube::audio::Effect::Thrust, true);
    else
      m_audio.stopEffect(geocube::audio::Effect::Thrust);
    if (m_ui.settings().effects &&
        inputState.isHeld(geocube::core::Action::Shield) &&
        m_world.shieldRemainingSeconds() > 0.0)
      m_audio.playEffect(geocube::audio::Effect::Shield, true);
    else
      m_audio.stopEffect(geocube::audio::Effect::Shield);
    geocube::core::InputState simulationInput = inputState;
    if (command.pauseMenuOpened)
      simulationInput.clear(geocube::core::Action::Quit);
    if (command.resumeGame) {
      simulationInput.clear(geocube::core::Action::Quit);
      simulationInput.press(geocube::core::Action::Confirm);
    }
    if (!command.pauseMenuOpened && !command.endGame &&
        (wasPlaying || m_ui.screen() == geocube::ui::Screen::Playing))
      m_world.advance(deltaSeconds, simulationInput);
    if (!m_ui.settings().music) {
      m_audio.stopMusic();
      m_activeMusic.clear();
    } else if (m_ui.screen() == geocube::ui::Screen::Playing &&
               m_activeMusic != m_world.currentLevel().music) {
      m_audio.playMusic(m_world.currentLevel().music);
      m_activeMusic = m_world.currentLevel().music;
    }
    if (m_ui.screen() == geocube::ui::Screen::Playing &&
        m_world.state() == geocube::core::GameState::GameOver)
      m_ui.handleInput({}, m_world);
    m_renderer.render(m_world);
    m_ui.draw(m_renderer, m_world);
    m_shell.present();
    ++m_renderedFrames;
    ++m_fpsFrames;
    m_fpsElapsed += deltaSeconds;

    if (m_fpsElapsed >= 1.0) {
      std::cout << "FPS: " << m_fpsFrames / m_fpsElapsed << '\n';
      m_fpsElapsed = 0.0;
      m_fpsFrames = 0;
    }
    if (m_options.frameLimit > 0 &&
        m_renderedFrames >= m_options.frameLimit)
      m_quit = true;
    return !m_quit;
  }

private:
  Options m_options;
  geocube::platform::SdlShell m_shell;
  geocube::render::Renderer m_renderer;
  geocube::platform::SettingsStore m_settingsStore;
  geocube::platform::Settings m_settings;
  geocube::ui::UiController m_ui;
  geocube::core::GameWorld m_world;
  geocube::audio::AudioSystem m_audio;
  geocube::input::SdlInput m_input;
  std::filesystem::path m_root;
  std::string m_activeMusic;
  geocube::platform::FrameClock m_clock;
  double m_fpsElapsed = 0.0;
  int m_fpsFrames = 0;
  int m_renderedFrames = 0;
  bool m_quit = false;
};

#ifdef __EMSCRIPTEN__
void runWebFrame(void* userData)
{
  auto* application = static_cast<Application*>(userData);
  if (!application->frame()) {
    emscripten_cancel_main_loop();
    delete application;
  }
}
#endif

int main(int argc, char** argv)
{
  Options options;
  if (!parseOptions(argc, argv, options))
    return 2;

  if (options.headless)
    return runHeadless();
  auto application = std::make_unique<Application>(std::move(options));
  if (!application->initialize())
    return 1;
#ifdef __EMSCRIPTEN__
  Application* persistentApplication = application.release();
  emscripten_set_main_loop_arg(runWebFrame, persistentApplication, 0, true);
  return 0;
#else
  while (application->frame()) {}
  return 0;
#endif
}
