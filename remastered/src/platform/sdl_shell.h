#ifndef GEOCUBE_REMASTERED_SDL_SHELL_H
#define GEOCUBE_REMASTERED_SDL_SHELL_H

#include <SDL3/SDL.h>

#include <string>

namespace geocube::platform {

struct ShellEvents {
  bool quit = false;
  bool resized = false;
  bool focusChanged = false;
  bool focused = true;
  bool fullscreenChanged = false;
};

using EventCallback = void (*)(const SDL_Event& event, void* userData);

class SdlShell final {
public:
  SdlShell() = default;
  ~SdlShell();

  SdlShell(const SdlShell&) = delete;
  SdlShell& operator=(const SdlShell&) = delete;

  bool initialize(const char* title, int width, int height);
  void shutdown();

  ShellEvents processEvents(EventCallback callback = nullptr,
                            void* userData = nullptr);
  void renderFrame();
  void present();
  void windowSizeInPixels(int& width, int& height) const;
  bool setFullscreen(bool fullscreen);
  void setTextInput(bool enabled);

  static void* glProcAddress(const char* name);

  const std::string& glVersion() const { return m_glVersion; }
  bool focused() const { return m_focused; }

private:
  bool toggleFullscreen();

  SDL_Window* m_window = nullptr;
  SDL_GLContext m_context = nullptr;
  std::string m_glVersion;
  bool m_focused = true;
  bool m_fullscreen = false;
};

} // namespace geocube::platform

#endif
