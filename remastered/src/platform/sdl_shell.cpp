#include "platform/sdl_shell.h"

#include <SDL3/SDL_opengl.h>

#include <iostream>

namespace geocube::platform {

SdlShell::~SdlShell()
{
  shutdown();
}

bool SdlShell::initialize(const char* title, int width, int height)
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL video initialization failed: " << SDL_GetError() << '\n';
    return false;
  }

#if defined(__EMSCRIPTEN__)
  constexpr int requestedMajor = 3;
  constexpr int requestedMinor = 0;
#elif defined(__APPLE__)
  constexpr int requestedMajor = 3;
  constexpr int requestedMinor = 2;
#else
  constexpr int requestedMajor = 3;
  constexpr int requestedMinor = 3;
#endif

  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, requestedMajor) ||
      !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, requestedMinor) ||
#if defined(__EMSCRIPTEN__)
      !SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                           SDL_GL_CONTEXT_PROFILE_ES) ||
#else
      !SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                           SDL_GL_CONTEXT_PROFILE_CORE) ||
#endif
      !SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) {
    std::cerr << "OpenGL attribute setup failed: " << SDL_GetError() << '\n';
    shutdown();
    return false;
  }

  m_window = SDL_CreateWindow(title, width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!m_window) {
    std::cerr << "SDL window creation failed: " << SDL_GetError() << '\n';
    shutdown();
    return false;
  }

  m_context = SDL_GL_CreateContext(m_window);
  if (!m_context) {
    std::cerr << "OpenGL context creation failed: " << SDL_GetError() << '\n';
    shutdown();
    return false;
  }

  SDL_GL_SetSwapInterval(1);
  const GLubyte* version = glGetString(GL_VERSION);
  m_glVersion = version ? reinterpret_cast<const char*>(version) : "unknown";
  return true;
}

void SdlShell::shutdown()
{
  if (m_context) {
    SDL_GL_DestroyContext(m_context);
    m_context = nullptr;
  }
  if (m_window) {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }
  SDL_Quit();
}

ShellEvents SdlShell::processEvents(EventCallback callback, void* userData)
{
  ShellEvents result;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (callback)
      callback(event, userData);
    switch (event.type) {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      result.quit = true;
      break;

    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      result.resized = true;
      break;

    case SDL_EVENT_WINDOW_FOCUS_GAINED:
      m_focused = true;
      result.focusChanged = true;
      result.focused = true;
      break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
      m_focused = false;
      result.focusChanged = true;
      result.focused = false;
      break;

    case SDL_EVENT_KEY_DOWN:
      if (!event.key.repeat && event.key.key == SDLK_F11) {
        result.fullscreenChanged = toggleFullscreen();
      }
      break;

    default:
      break;
    }
  }
  return result;
}

void SdlShell::renderFrame()
{
  int width = 0;
  int height = 0;
  windowSizeInPixels(width, height);
  glViewport(0, 0, width, height);
  glClearColor(0.015f, 0.025f, 0.07f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  present();
}

void SdlShell::present()
{
  SDL_GL_SwapWindow(m_window);
}

void SdlShell::windowSizeInPixels(int& width, int& height) const
{
  SDL_GetWindowSizeInPixels(m_window, &width, &height);
}

void SdlShell::setTextInput(bool enabled)
{
  if (enabled)
    SDL_StartTextInput(m_window);
  else
    SDL_StopTextInput(m_window);
}

void* SdlShell::glProcAddress(const char* name)
{
  return reinterpret_cast<void*>(SDL_GL_GetProcAddress(name));
}

bool SdlShell::toggleFullscreen()
{
  return setFullscreen(!m_fullscreen);
}

bool SdlShell::setFullscreen(bool fullscreen)
{
  if (!SDL_SetWindowFullscreen(m_window, fullscreen)) {
    std::cerr << "Fullscreen toggle failed: " << SDL_GetError() << '\n';
    return false;
  }
  m_fullscreen = fullscreen;
  return true;
}

} // namespace geocube::platform
